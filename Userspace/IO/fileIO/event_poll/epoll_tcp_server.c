#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include "hash.h"

/* /sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size
   getconf LEVEL1_DCACHE_LINESIZE */
#define CACHE_LINE_SIZE       64

#define MAX_EVENTS            1000
#define MAX_CONNECTED_TIME    5*60  //5 mins

#define STAT_TBL_ENTRIES      0x40000

/* TCP keepalive */
#define TCP_KEEPALIVE_TIME    300
#define TCP_KEEPALIVE_INTVL   60
#define TCP_KEEPALIVE_PROBES  3

typedef struct hash_key_s {
    uint32_t local_ip;
    uint32_t remote_ip;
    uint16_t local_port;
    uint16_t remote_port;
} __attribute__((__packed__)) hash_key_t;

typedef struct connection_table_s {
    time_t expired_time[STAT_TBL_ENTRIES];  //tcp slow attack
} __attribute__((aligned (CACHE_LINE_SIZE))) connection_table_t;

connection_table_t connection_stat_tbl;

static uint32_t get_socket_index(int sockfd)
{
    uint32_t hash;
    struct sockaddr_in serv, guest;
    socklen_t len = sizeof(struct sockaddr_in);

    /* getsockname - get socket name */
    if (getsockname(sockfd, (struct sockaddr *)&serv, &len) != 0) {
        perror("getsockname");
        return -1;
    }

    /* getpeername - get name of connected peer socket */
    if (getpeername(sockfd, (struct sockaddr *)&guest, &len) != 0) {
        perror("getpeername");
        return -1;
    }

    hash_key_t hash_key;

    hash_key.local_ip    = serv.sin_addr.s_addr;
    hash_key.remote_ip   = guest.sin_addr.s_addr;
    hash_key.local_port  = serv.sin_port;
    hash_key.remote_port = guest.sin_port;
    hash = jhash2((u32 *)&hash_key, sizeof(hash_key)/sizeof(__u32), 0);

    char serv_ip[32], guest_ip[32];
    /* inet_ntop - convert IPv4 and IPv6 addresses from binary to text form */
    inet_ntop(AF_INET, &serv.sin_addr, serv_ip, sizeof(serv_ip));
    inet_ntop(AF_INET, &guest.sin_addr, guest_ip, sizeof(guest_ip));

    printf("Local (%s:%d), Foreign (%s:%d), hash (%u)\n",
           serv_ip, ntohs(serv.sin_port),
           guest_ip, ntohs(guest.sin_port), hash);

    return hash%STAT_TBL_ENTRIES;
}

static int set_socket_keepalive(int sockfd, int idle, int intvl, int probes)
{
    int optval;
    socklen_t optlen = sizeof(optval);

    /* Set the option active */
    optval = 1;
    optlen = sizeof(optval);
    if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
        perror("setsockopt(SO_KEEPALIVE)");
        return -1;
    }

    if (setsockopt(sockfd, SOL_TCP, TCP_KEEPIDLE, &idle, sizeof(int)) != 0) {
        perror("setsockopt(TCP_KEEPIDLE)");
        return -1;
    }

    if (setsockopt(sockfd, SOL_TCP, TCP_KEEPINTVL, &intvl, sizeof(int)) != 0) {
        perror("setsockopt(TCP_KEEPINTVL)");
        return -1;
    }

    if (setsockopt(sockfd, SOL_TCP, TCP_KEEPCNT, &probes, sizeof(int)) != 0) {
        perror("setsockopt(TCP_KEEPCNT)");
        return -1;
    }

    return 0;;
}

/* Mandatory locks are enforced for all processes.
   If a process tries to perform an incompatible access
   (e.g., read(2) or write(2)) on a file region that
   has an incompatible mandatory lock,
   then the result depends upon whether the O_NONBLOCK flag is enabled
   for its open file description.

   O_NONBLOCK flag
   Enable:  system call fails with the error EAGAIN.
   Disable: system call is blocked until the lock is removed or
            converted to a mode that is compatible with the access. */
static int set_socket_nonblocking(int sockfd)
{
    int flags, ret;

    /* fcntl - manipulate file descriptor */
    flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl(F_GETFL)");
        return -1;
    }

    flags |= O_NONBLOCK;
    ret = fcntl(sockfd, F_SETFL, flags);
    if (ret == -1) {
        perror("fcntl(F_SETFL)");
        return -1;
    }

    return 0;
}

static int init_connection(int sockfd)
{
    /* Make the incoming socket non-blocking */
    if (set_socket_nonblocking(sockfd) == -1)
        return -1;

    /* TCP keepalive */
    if (set_socket_keepalive(sockfd,
                             TCP_KEEPALIVE_TIME,
                             TCP_KEEPALIVE_INTVL,
                             TCP_KEEPALIVE_PROBES) == -1)
        return -1;

    /* uptime stat */
    int index = get_socket_index(sockfd);
    if (index >= 0)
        connection_stat_tbl.expired_time[index] = time(NULL) + MAX_CONNECTED_TIME;
    else
        return -1;

    return 0;
}

static int check_connection_uptime(int sockfd)
{
    int index = get_socket_index(sockfd);
    if (index >= 0) {
        if (time(NULL) > connection_stat_tbl.expired_time[index]) {
            printf("Connection expired, descriptor (%d)\n", sockfd);
            return -1;
        }
    } else
        return -1;

    return 0;
}

/* Some daemons need root permissions to start.
   This happens for example when a daemon wants to bind to a low port (<1024). */
static int init_server(char *port)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int ret, sockfd;

    /* Obtain address(es) matching host/port */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family   = AF_UNSPEC;   /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* TCP socket */
    hints.ai_flags    = AI_PASSIVE;  /* All interfaces */

    /* getaddrinfo, freeaddrinfo, gai_strerror - network address and service translation */
    ret = getaddrinfo(NULL, port, &hints, &result);
    if (ret != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        return -1;
    }

    /* getaddrinfo() returns a list of address structures.
       Try each address until we successfully bind(2).
       If socket(2) (or bind(2)) fails, we (close the socket
       and) try the next address. */

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        /* socket - create an endpoint for communication */
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd == -1)
            continue;

        /* bind: Address already in use */
        int reuse = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
            perror("setsockopt(SO_REUSEADDR)");

        /* bind - bind a name to a socket */
        if (bind(sockfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;  /* Success */

        close(sockfd);
    }

    /* No address succeeded */
    if (rp == NULL) {
        fprintf(stderr, "Could not bind\n");
        perror("bind");
        return -1;
    }

    freeaddrinfo(result);
    return sockfd;
}

int main(int argc, char *argv[])
{
    int ret, sockfd, epfd;
    struct epoll_event  event;
    struct epoll_event *events;
    struct rlimit limit;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s [port]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* > 4096 need root permission */
    limit.rlim_cur = STAT_TBL_ENTRIES;  /* Soft limit: /proc/<PID>/fd */
    limit.rlim_max = STAT_TBL_ENTRIES;  /* Hard limit (ceiling for rlim_cur) */
    /* Specifies a value one greater than the maximum file descriptor number that can be opened by this process. */
    if (setrlimit(RLIMIT_NOFILE, &limit) != 0) {
        perror("setrlimit");
        exit(EXIT_FAILURE);
    }

    /* Get max number of files. */
    if (getrlimit(RLIMIT_NOFILE, &limit) != 0) {
        perror("getrlimit");
        exit(EXIT_FAILURE);
    }

    sockfd = init_server(argv[1]);
    if (sockfd < 0) {
        fprintf(stderr, "create_and_bind fail (%d)\n", sockfd);
        exit(EXIT_FAILURE);
    }

    ret = set_socket_nonblocking(sockfd);
    if (ret == -1) {
        fprintf(stderr, "set_socket_nonblocking fail\n");
        exit(EXIT_FAILURE);
    }

    /* listen - listen for connections on a socket.
       The backlog argument defines the maximum length to which
       the queue of pending connections for sockfd may grow.
       If a connection request arrives when the queue is full,
       the client may receive an error with an indication of ECONNREFUSED or,
       if the underlying protocol supports retransmission,
       the request may be ignored so that a later reattempt at connection succeeds.

       If the backlog argument is greater than the value in
       /proc/sys/net/core/somaxconn, then it is silently truncated to that value;
       the default value in this file is 128. In kernels before 2.4.25,
       this limit was a hard coded value, SOMAXCONN, with the value 128. */
    ret = listen(sockfd, SOMAXCONN);
    if (ret == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* epoll_create1 - open an epoll file descriptor */
    epfd = epoll_create1(0);
    if (epfd < 0) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    event.data.fd = sockfd;

    /* EPOLLIN
       The associated file is available for read(2) operations.

       EPOLLET
       Sets the Edge Triggered behavior for the associated file descriptor.
       The default behavior for epoll is Level Triggered.
       See epoll(7) for more detailed information about
       Edge and Level Triggered event distribution architectures. */
    event.events = EPOLLIN | EPOLLET;

    /* epoll_ctl - control interface for an epoll descriptor */
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);
    if (ret) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    events = calloc(MAX_EVENTS, sizeof(event));

    while (1) {
        int i, number;

        /* epoll_wait, epoll_pwait - wait for an I/O event on an epoll file descriptor */
        number = epoll_wait(epfd, events, MAX_EVENTS, -1);

        for (i = 0; i < number; i++) {
            if ((events[i].events & EPOLLERR)||
                    (events[i].events & EPOLLHUP)||
                    (!(events[i].events & EPOLLIN))) {
                /* An error has occured on this fd, or the socket is not
                   ready for reading (why were we notified then?) */
                fprintf(stderr,"epoll error\n");
                close(events[i].data.fd);
                continue;
            } else if (sockfd == events[i].data.fd) {
                /* We have a notification on the listening socket, which
                   means one or more incoming connections. */
                while (1) {
                    int connfd;
                    struct sockaddr in_addr;
                    socklen_t in_len = sizeof(in_addr);
                    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

                    /* accept: Too many open files */
                    connfd = accept(sockfd, &in_addr, &in_len);

                    if (connfd == -1) {
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                            /* We have processed all incoming connections. */
                            break;
                        } else {
                            perror("accept");
                            break;
                        }
                    }

                    /* getnameinfo - address-to-name translation in protocol-independent manner
                       NI_NUMERICHOST: If set, then the numeric form of the hostname is returned.
                       NI_NUMERICSERV: If set, then the numeric form of the service address is returned. */
                    if (getnameinfo(&in_addr, in_len, hbuf, sizeof(hbuf), sbuf,
                                    sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV) == 0)
                        printf("Accepted connection on descriptor %d (host=%s, port=%s)\n",
                               connfd, hbuf, sbuf);

                    ret = init_connection(connfd);
                    if (ret == -1) {
                        close(connfd);
                        break;
                    }

                    event.data.fd = connfd;
                    event.events = EPOLLIN | EPOLLET;
                    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &event);
                    if (ret == -1) {
                        perror("epoll_ctl");
                        close(connfd);
                    }
                }
                continue;
            } else {
                /* We have data on the fd waiting to be read. Read and
                   display it. We must read whatever data is available
                   completely, as we are running in edge-triggered mode
                   and won't get a notification again for the same data. */
                if (check_connection_uptime(events[i].data.fd) == -1) {
                    close(events[i].data.fd);
                    continue;
                }

                int done = 0;
                while (1) {
                    ssize_t count;
                    char buf[512];

                    count = read(events[i].data.fd, buf, sizeof(buf));
                    if (count == -1) {
                        /* If errno == EAGAIN, that means we have read all
                           data. So go back to the main loop. */
                        if (errno != EAGAIN) {
                            perror("read");
                            done = 1;
                        }
                        break;
                    } else if (count == 0) {
                        /* End of file. The remote has closed the
                           connection. */
                        done = 1;
                        break;
                    }
                    /* Write the buffer to standard output */
                    ret = write(1, buf, count);
                    if (ret == -1) {
                        perror("write");
                        break;
                    }
                }
                if (done) {
                    printf("Closed connection on descriptor %d\n", events[i].data.fd);
                    /* Closing the descriptor will make epoll remove it
                       from the set of descriptors which are monitored. */
                    close(events[i].data.fd);
                }
            }
        }
    }

    free(events);
    close(sockfd);
    return EXIT_SUCCESS;
}

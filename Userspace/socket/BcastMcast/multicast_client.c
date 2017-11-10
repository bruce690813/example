#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MSGBUFSIZE  2048
#define HELLO_PORT  12345
#define HELLO_GROUP "225.0.0.37"

int main()
{
    int fd, nbytes;
    char msgbuf[MSGBUFSIZE];
    struct sockaddr_in addr;
    struct ip_mreq mreq;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(HELLO_PORT);

    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }

    u_int yes = 1;
    /* allow multiple sockets to use the same PORT number */
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        perror("SO_REUSEADDR");
        exit(1);
    }

    /* use setsockopt() to request that the kernel join a multicast group */
    mreq.imr_multiaddr.s_addr = inet_addr(HELLO_GROUP);  /* known multicast group */
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);  /* network interface */
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("IP_ADD_MEMBERSHIP");
        exit(1);
    }

    while (1) {
        socklen_t addrlen = sizeof(addr);
        if ((nbytes = recvfrom(fd, msgbuf, MSGBUFSIZE, 0,
                               (struct sockaddr *) &addr, &addrlen)) < 0) {
            perror("recvfrom");
            exit(1);
        }
        printf("nbytes(%d) message = %s\n", nbytes, msgbuf);
    }

    return 0;
}

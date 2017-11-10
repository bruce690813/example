#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MSGBUFSIZE   2048
#define HELLO_PORT   53
#define UNICAST_ADDR "127.0.0.1"

int main()
{
    int fd, client_fd, nbytes;
    char message[MSGBUFSIZE];
    struct sockaddr_in addr, client_addr;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    u_int yes = 1;
    /* allow multiple sockets to use the same PORT number */
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        perror("SO_REUSEADDR");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(UNICAST_ADDR);
    addr.sin_port = htons(HELLO_PORT);

    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }

    if (listen(fd, 20) < 0) {
        perror("listen");
        exit(1);
    }

    int c = sizeof(struct sockaddr_in);
    client_fd = accept(fd, (struct sockaddr *)&client_addr, (socklen_t*)&c);
    if (client_fd < 0) {
        perror("accept");
        exit(1);
    }

    while (1) {
        memset(message, 0, sizeof(message));
        if ((nbytes = recv(client_fd, message, sizeof(message), 0)) < 0) {
            perror("recv");
            exit(1);
        }

        if (nbytes > 0)
            printf("nbytes(%d) message = %s\n", nbytes, message);
    }

    return 0;
}

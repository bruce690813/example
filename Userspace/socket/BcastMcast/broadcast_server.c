#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MSGBUFSIZE  2048
#define HELLO_PORT  12345
#define BROAD_ADDR "255.255.255.255"

int main()
{
    int fd, nbytes;
    char message[MSGBUFSIZE] = "Hello, World!";
    struct sockaddr_in addr;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(BROAD_ADDR);
    addr.sin_port = htons(HELLO_PORT);

    u_int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes)) < 0) {
        perror("SO_BROADCAST");
        exit(1);
    }

    while (1) {
        if ((nbytes = sendto(fd, message, strlen(message), 0,
                             (struct sockaddr *) &addr, sizeof(addr))) < 0) {
            perror("sendto");
            exit(1);
        }
        sleep(1);
    }

    return 0;
}

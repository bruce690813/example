#include <sys/socket.h>
#include <sys/types.h>          /* See NOTES */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MSGBUFSIZE  4
#define HELLO_PORT  53
#define SERVER_ADDR "207.192.176.32"

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

int main()
{
    int fd, nbytes;
    char msgbuf[MSGBUFSIZE];
    struct sockaddr_in addr;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    addr.sin_port = htons(HELLO_PORT);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(1);
    }

    while (1) {
        rand_string(msgbuf, MSGBUFSIZE);
        if ((nbytes = write(fd, msgbuf, strlen(msgbuf))) < 0) {
            perror("recvfrom");
            exit(1);
        }
        printf("nbytes(%d) message = %s\n", nbytes, msgbuf);
        sleep(15);
    }

    return 0;
}

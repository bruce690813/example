#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MSGBUFSIZE  2048
#define HELLO_PORT  12345
#define HELLO_GROUP "225.0.0.37"

void main(int argc, char *argv[])
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
    addr.sin_addr.s_addr = inet_addr(HELLO_GROUP);
    addr.sin_port = htons(HELLO_PORT);
     
    while (1) {
        if ((nbytes = sendto(fd, message, strlen(message), 0, 
                   (struct sockaddr *) &addr, sizeof(addr))) < 0) {
	        perror("sendto");
	        exit(1);
	    }
	    sleep(1);
    }
}

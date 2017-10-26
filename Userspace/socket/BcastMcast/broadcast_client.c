#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MSGBUFSIZE  2048
#define HELLO_PORT  12345
#define BROAD_ADDR "255.255.255.255"

void main(int argc, char *argv[])
{
    int fd, nbytes, addrlen;
    char msgbuf[MSGBUFSIZE];
    struct sockaddr_in addr;
    struct ip_mreq mreq;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	    perror("socket");
	    exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(BROAD_ADDR);
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
  
    while (1) {
	    addrlen = sizeof(addr);
	    if ((nbytes = recvfrom(fd, msgbuf, MSGBUFSIZE, 0,
			       (struct sockaddr *) &addr, &addrlen)) < 0) {
	        perror("recvfrom");
	        exit(1);
	    }
        printf("nbytes(%d) message = %s\n", nbytes, msgbuf);
    }
}

CFLAGS += -g -Wall -Wunused
CFLAGS += -Wextra -Wcast-qual -Werror

TARGETS = epoll_tcp_server 

TARGETS_SRC += \
		     epoll_tcp_server.c \

TARGETS_OBJ = $(TARGETS_SRC:.c=.o)	

$(TARGETS): $(TARGETS_OBJ)
	gcc $(CFLAGS) -o $@ $^

all: $(TARGETS) 

clean:
	rm -f $(TARGETS) *~ *.o *.out

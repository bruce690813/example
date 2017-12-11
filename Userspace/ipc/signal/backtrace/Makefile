CFLAGS += -g -Wall -Wunused
CFLAGS += -Wextra -Wcast-qual -Werror
CFLAGS += -rdynamic

TARGETS = test_backtrace

TARGETS_SRC += \
		     main.c \
		     backtrace.c \

TARGETS_OBJ = $(TARGETS_SRC:.c=.o)	

$(TARGETS): $(TARGETS_OBJ)
	gcc $(CFLAGS) -o $@ $^

all: $(TARGETS) 

clean:
	rm -f $(TARGETS) *~ *.o *.out

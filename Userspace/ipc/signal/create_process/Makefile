CFLAGS += -g -Wall -Wunused
CFLAGS += -Wextra -Wcast-qual -Werror

TARGET1 = create_process
TARGET2 = cmd_process

TARGET1_SRC += \
             create_process.c \
             ipc_signal.c

TARGET2_SRC += \
             cmd_process.c \
             ipc_signal.c

TARGET1_OBJ = $(TARGET1_SRC:.c=.o)
TARGET2_OBJ = $(TARGET2_SRC:.c=.o)

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(TARGET1_OBJ)
	gcc $(CFLAGS) -o $@ $^

$(TARGET2): $(TARGET2_OBJ)
	gcc $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET1) $(TARGET2) *~ *.o *.out

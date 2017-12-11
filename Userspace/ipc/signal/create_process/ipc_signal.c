#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

static void signal_handler(int signo)
{
    if (signo == SIGINT)
        printf("PID(%d): Caught SIGINT!\n", getpid());
    else if (signo == SIGTERM)
        printf("PID(%d): Caught SIGTERM!\n", getpid());
    else {
        fprintf(stderr, "Unexpected signal!\n");
    }
}

int init_signals()
{
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        fprintf(stderr, "Cannot handle SIGINT!\n");
        return -1;
    }

    if (signal(SIGTERM, signal_handler) == SIG_ERR) {
        fprintf(stderr, "Cannot handle SIGINT!\n");
        return -1;
    }

    return 0;
}

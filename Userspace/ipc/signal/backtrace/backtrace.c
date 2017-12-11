#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <execinfo.h>
#include <time.h>

#define SIZE 				16
#define handle_error(msg) \
               do { perror(msg); return(EXIT_FAILURE); } while (0)

extern const char * const sys_siglist[];

static void get_localtime(time_t *timep, struct tm *mytime)
{
    time(timep);
    tzset();
    localtime_r(timep, mytime);
}

static void show_stackframe()
{
    int i, trace_size;
    void *trace[SIZE];
    char **messages = (char **)NULL;

    trace_size = backtrace(trace, SIZE);
    printf("backtrace() returned %d addresses\n", trace_size);

    time_t timep;
    struct tm mytime;
    get_localtime(&timep, &mytime);
    printf("[Time %d/%02d/%02d %02d:%02d:%02d] Call Trace:\n",
           (1900+mytime.tm_year), (1+mytime.tm_mon), (mytime.tm_mday),
           (mytime.tm_hour), (mytime.tm_min), (mytime.tm_sec));

    messages = backtrace_symbols(trace, trace_size);
    for (i = 0; i < trace_size; i++) {
        printf("[Backtrace] %s\n", messages[i]);
    }
}

static void signal_handler(int signo, siginfo_t *info, void *unused)
{
    (void)info;
    (void)unused;

    printf("Caught signal(%d): %s\n", signo, sys_siglist[signo]);
    show_stackframe();
}

int init_signals()
{
    struct sigaction sig_term;

    memset(&sig_term, 0, sizeof(struct sigaction));
    if (sigemptyset(&sig_term.sa_mask) == 0) {
        sig_term.sa_sigaction = signal_handler;
        sig_term.sa_flags = SA_SIGINFO|SA_RESETHAND;

        if (sigaction(SIGSEGV, &sig_term, NULL) != 0)
            handle_error("sigaction(SIGSEGV)");
    } else {
        printf("sigemptyset(): ERROR!!!");
        return EXIT_FAILURE;
    }
    /*
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGINT);
        sigaddset(&set, SIGTERM);
        sigaddset(&set, SIGCHLD);
        sigaddset(&set, SIGQUIT);

        if (sigprocmask(SIG_SETMASK, &set, NULL) == -1)
            goto failed;
    */
    return EXIT_SUCCESS;
}

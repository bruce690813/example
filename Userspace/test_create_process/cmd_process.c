#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#define LOOP_NUM  5
#define SLEEP_SEC 5

static inline void ps_log(const char * format, ...)
{
    char slog[512];
    memset(slog, 0, sizeof(slog));
    va_list arg;
    va_start(arg, format);
    vsprintf(slog, format, arg);
    va_end(arg);

    printf("[PID(%d)_PPID(%d)] %s", getpid(), getppid(), slog);
}

int main()
{
    int i = 0;

    sleep(SLEEP_SEC);
    ps_log("%s: start\n", __FILE__);

    while (i < LOOP_NUM) {
        ps_log("%s: i = %d\n", __FILE__, i);
        i++;
        sleep(SLEEP_SEC);
    }

    ps_log("%s: end\n", __FILE__);
    return 0;
}

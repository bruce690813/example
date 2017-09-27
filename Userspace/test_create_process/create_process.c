#include <stdio.h>
#include <stdlib.h>  //system
#include <string.h>
#include <stdarg.h>
#include <unistd.h>  //ececl
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>  //waitpid
#include <sys/wait.h>  //waitpid

#define LOOP_NUM  5
#define SLEEP_SEC 5

static inline void ps_log(const char * format, ...)
{
    char slog[1024];
    memset(slog, 0, sizeof(slog));
    va_list arg;
    va_start(arg, format);
    vsprintf(slog, format, arg);
    va_end(arg);

    printf("[PID(%d)_PPID(%d)] %s", getpid(), getppid(), slog);
}

int child_process()
{
    int i = 0;

    sleep(SLEEP_SEC);
    ps_log("%s: start\n", __func__);

    while (i < LOOP_NUM) {
        ps_log("%s: i = %d\n", __func__, i);
        i++;
        sleep(SLEEP_SEC);
    }

    ps_log("%s: end\n", __func__);
    return 0;
}

int grandchild_process()
{
    int i = 0;

    sleep(SLEEP_SEC);
    ps_log("%s: start\n", __func__);

    while (i < LOOP_NUM) {
        ps_log("%s: i = %d\n", __func__, i);
        i++;
        sleep(SLEEP_SEC);
    }

    ps_log("%s: end\n", __func__);
    return 0;
}

int main()
{
    pid_t pid;
    ps_log("%s: start\n", __FILE__);

    /* system - execute a shell command */
    ps_log("=== test system() ===\n");
    system("./cmd_process");
    ps_log("%s: system return\n", __FILE__);

    /* fork - create a child process */
    printf("\n");
    ps_log("=== test fork() ===\n");
    sleep(SLEEP_SEC);

    if ((pid = fork())) {
        /* Parent dosomething ... */
        ps_log("%s: parent dosmething...\n", __FILE__);
        sleep(10*SLEEP_SEC);

        /* wait, waitpid, waitid - wait for process to change state */
        int status;
        waitpid(pid, &status, 0);
    } else if (pid == 0) {
        child_process();
        /* zombie process is a process has completed execution
           (via the exit system call) but still has an entry in the process table:
           it is a process in the "Terminated state". */
        _exit(EXIT_SUCCESS);
    } else {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    /* double fork to avoid zombie process */
    printf("\n");
    ps_log("=== double fork to avoid zombie process ===\n");
    sleep(SLEEP_SEC);

    if ((pid = fork())) {
        /* wait, waitpid, waitid - wait for process to change state */
        int status;
        waitpid(pid, &status, 0);

        /* Parent dosomething ... */
        ps_log("%s: parent dosmething...\n", __FILE__);
        sleep(10*SLEEP_SEC);
    } else if (pid == 0) {
        ps_log("child_process\n");
        /* child process */
        if (fork())
            _exit(EXIT_SUCCESS);
        else {
            /* child process */
            grandchild_process();
            _exit(EXIT_SUCCESS);
        }
    } else {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    printf("\n");
    ps_log("=== test execl ===\n");
    sleep(SLEEP_SEC);
    execl("./cmd_process", "cmd_process", (char *)0);

    /* do not execute */
    ps_log("=== test execl ===\n");
    /* int execl(const char *path, const char *arg, ... (char  *) NULL); */
    execl("/bin/ls", "ls", "-al", "/etc/passwd", (char *)0);

    ps_log("=== test execlp ===\n");
    /* int execlp(const char *file, const char *arg, ... (char  *) NULL); */
    execlp("ls", "ls", "-al", "/etc/passwd", (char *)0);

    ps_log("=== test execv ===\n");
    char *argv[] = {"ls", "-al", "/etc/passwd", (char *)0};
    /* int execv(const char *path, char *const argv[]); */
    execv("/bin/ls", argv);

    ps_log("=== test execvp ===\n");
    /* int execvp(const char *file, char *const argv[]); */
    execvp("ls", argv);

    ps_log("=== test execve ===\n");
    /* int execve(const char *filename, char *const argv[], char *const envp[]); */
    char *envp[] = {"PATH=/bin", 0};
    execve("/bin/ls", argv, envp);

    ps_log("%s: end\n", __FILE__);
    return EXIT_SUCCESS;
}

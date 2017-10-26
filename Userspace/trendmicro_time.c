#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

time_t strtotime(char *str)
{
    int i = 0;
    char buf[3][32];
    char *token;
    char *delim = ":";
    time_t timep;
    struct tm tb;

    token = strtok(str, delim);
    while (token != NULL) {
        strcpy(buf[i], token);
        token = strtok(NULL, delim);
        i++;
    }

    tb.tm_hour = atoi(buf[0]);
    tb.tm_min = atoi(buf[1]);
    tb.tm_sec = atoi(buf[2]);
    timep = mktime(&tb);

    return timep;
}

int check(time_t timep)
{
    int i;
    int array[3];

    struct tm *p = gmtime(&timep);

    array[0] = p->tm_hour;
    array[1] = p->tm_min;
    array[2] = p->tm_sec;

    int xor_sum = array[0] ^ array[1] ^ array[2];

    for (i = 0; i < 3; i++) {
        if (xor_sum == array[i]) return 1;
    }

    return 0;
}

int soluction(char *start, char *end)
{
    int count = 0;
    time_t index, start_timep, end_timep;

    start_timep = strtotime(start);
    end_timep = strtotime(end);

    for (index = start_timep+1; index < end_timep; index++) {
        if (check(index))
            count++;
    }

    return count;
}

void main()
{
    char start_time[32] = "10:11:09";
    char end_time[32] = "10:12:11";

    int count = soluction(start_time, end_time);

    printf("count = %d\n", count);
}

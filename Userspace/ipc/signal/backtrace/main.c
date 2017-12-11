#include <stdio.h>
#include <stdlib.h>

#include "backtrace.h"

void func2()
{
    int *foo = (int*)-1;
    printf("%d\n", *foo);
}

void func1()
{
    func2();
}

int main()
{
    init_signals();

    func1();

    return EXIT_SUCCESS;
}

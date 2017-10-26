#include <stdio.h>
#include <string.h>

int soluction(char *str)
{
    int i, length, left, right;

    length = strlen(str);
    left = 0;
    right = length -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        //check
        int left_sum = 0, right_sum = 0;
        char *ptr = str;

        for (i = 0; i < length; i++) {
            if (i < mid && *ptr == '(')
                left_sum++;
            else if (i > mid && *ptr == ')')
                right_sum++;
            ptr++;
        }

        if (left_sum == right_sum) {
            printf("count = %d\n", left_sum);
            return mid;
        }
        if (left_sum > right_sum)  right -= 1;
        if (left_sum < right_sum)  left += 1;
    }

    return -1;
}

void main()
{
    char string[32] = "(())))(";  //index = 3;
    //char string[32] = "()()())))))";  //index = 7;
    //char string[32] = "()()(()))(())))))";  //not found

    printf("%s: index = %d\n", string, soluction(string));
}

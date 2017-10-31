/*
AbsDistinct: Compute number of distinct absolute values of sorted array elements.

A non-empty zero-indexed array A consisting of N numbers is given. The array is sorted in non-decreasing order. The absolute distinct count of this array is the number of distinct absolute values among the elements of the array.

For example, consider array A such that:

  A[0] = -5
  A[1] = -3
  A[2] = -1
  A[3] =  0
  A[4] =  3
  A[5] =  6
The absolute distinct count of this array is 5, because there are 5 distinct absolute values among the elements of this array, namely 0, 1, 3, 5 and 6.

Write a function:

int solution(int A[], int N);

that, given a non-empty zero-indexed array A consisting of N numbers, returns absolute distinct count of array A.

For example, given array A such that:

  A[0] = -5
  A[1] = -3
  A[2] = -1
  A[3] =  0
  A[4] =  3
  A[5] =  6
the function should return 5, as explained above.

Assume that:
N is an integer within the range [1..100,000];
each element of array A is an integer within the range [−2,147,483,648..2,147,483,647];
array A is sorted in non-decreasing order.

Complexity:
expected worst-case time complexity is O(N);
expected worst-case space complexity is O(N), beyond input storage (not counting the storage required for input arguments).
Elements of input arrays can be modified.

*/

#include <stdio.h>
#define ABS(X) ((X >= 0) ? (X) : (-X))

int solution(int A[], int N);
int solution(int A[], int N)
{
    int current = ABS(A[0]) > ABS(A[N - 1]) ? ABS(A[0]) : ABS(A[N - 1]), left;

    printf("current = %d\n", current);

    for (left = 0; left < N && A[left] == -2147483648; ++left) ;

    printf("left = %d\n", left);

    int result = left > 0 ? 1 : 0;
    for (int right = N - 1; left <= right; ++result) {
        for (; ABS(A[left]) == current; ++left) ;
        for (; ABS(A[right]) == current; --right) ;
        current = ABS(A[left]) > ABS(A[right]) ? ABS(A[left]) : ABS(A[right]);
        printf("current = %d, left = %d, right = %d, result = %d\n", current, left, right, result);
    }
    return result;
}

int main(void)
{
    //int A[] = {-5, -3, -1, 0, 6};
    int A[] = {-5, -3, -1, 0, 3, 6, 8, 100};
    printf("%d\n", solution(A, 8));
    return 0;
}


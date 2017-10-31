/*
You are given integers K, M and a non-empty zero-indexed array A consisting of N integers. Every element of the array is not greater than M.

You should divide this array into K blocks of consecutive elements. The size of the block is any integer between 0 and N. Every element of the array should belong to some block.

The sum of the block from X to Y equals A[X] + A[X + 1] + ... + A[Y]. The sum of empty block equals 0.

The large sum is the maximal sum of any block.

For example, you are given integers K = 3, M = 5 and array A such that:

  A[0] = 2
  A[1] = 1
  A[2] = 5
  A[3] = 1
  A[4] = 2
  A[5] = 2
  A[6] = 2
The array can be divided, for example, into the following blocks:

[2, 1, 5, 1, 2, 2, 2], [], [] with a large sum of 15;
[2], [1, 5, 1, 2], [2, 2] with a large sum of 9;
[2, 1, 5], [], [1, 2, 2, 2] with a large sum of 8;
[2, 1], [5, 1], [2, 2, 2] with a large sum of 6.
The goal is to minimize the large sum. In the above example, 6 is the minimal large sum.

Write a function:

int solution(int K, int M, int A[], int N);

that, given integers K, M and a non-empty zero-indexed array A consisting of N integers, returns the minimal large sum.

For example, given K = 3, M = 5 and array A such that:

  A[0] = 2
  A[1] = 1
  A[2] = 5
  A[3] = 1
  A[4] = 2
  A[5] = 2
  A[6] = 2
the function should return 6, as explained above.

Assume that:
N and K are integers within the range [1..100,000];
M is an integer within the range [0..10,000];
each element of array A is an integer within the range [0..M].

Complexity:
expected worst-case time complexity is O(N*log(N+M));
expected worst-case space complexity is O(1), beyond input storage (not counting the storage required for input arguments).
Elements of input arrays can be modified.

Detected time complexity:
O(N*log(N+M))
expand allExample tests
▶ example
example test ✔OK
expand allCorrectness tests
▶ extreme_single
single elements ✔OK
▶ extreme_double
single and double elements ✔OK
▶ extreme_min_max
maximal / minimal values ✔OK
▶ simple1
simple tests ✔OK
▶ simple2
simple tests ✔OK
▶ tiny_random_ones
random values {0, 1}, N = 100 ✔OK
expand allPerformance tests
▶ small_random_ones
random values {0, 1}, N = 100 ✔OK
▶ medium_zeros
many zeros and 99 in the middle, length = 15,000 ✔OK
▶ medium_random
random values {1, 100}, N = 20,000 ✔OK
▶ large_random
random values {0, ..., MAX_INT}, N = 100,000 ✔OK
▶ large_random_ones
random values {0, 1}, N = 100,000 ✔OK
▶ all_the_same
all the same values, N = 100,000 ✔OK
*/

#include <stdio.h>

int check(int A[], int N, int K, int mid)
{
    //printf("\nmid = %d\n", mid);

    int sum = 0;
    int blocks = 0;

    for (int i = 0; i < N; ++i) {
        //printf("sum = %d, blocks = %d, A[i] = %d\n", sum, blocks, A[i]);
        if ((sum + A[i]) > mid) {
            //printf("[%d] ! sum = %d, A[i] = %d\n", i, sum, A[i]);
            sum = A[i];

            if (++blocks >= K) {
                //printf("return 0\n");
                return 0;
            }

        } else {
            sum += A[i];
        }
    }

    //printf("return 1\n");
    return 1;
}

int solution(int K, int M, int A[], int N)
{
    int min_sum = 0;
    int max_sum = 0;

    // Calculate sum of total array (max_sum)
    // and max value in array (min_sum)
    for (int i = 0; i < N; ++i) {
        max_sum += A[i];

        if (A[i] > min_sum) {
            min_sum = A[i];
        }
    }

    if (K == 1) {
        // Split only into 1 block so
        // return total sum
        return max_sum;
    } else if (K == N) {
        // Split into N many blocks
        // so return max value
        return min_sum;
    }

    int mid;

    while (min_sum <= max_sum) {
        //printf("min_sum = %d, max_sum = %d\n", min_sum, max_sum);
        mid = (min_sum + max_sum) / 2;
        //mid = min_sum + (max_sum – min_sum)/2;

        if (check(A, N, K, mid)) {
            max_sum = mid - 1;
        } else {
            min_sum = mid + 1;
        }
    }

    return min_sum;
}

void main()
{
    int A[] = {2, 1, 5, 1, 2, 2, 2};

    int min_sum = solution(3, 5, A, sizeof(A)/sizeof(int));
    printf("min_sum = %d\n", min_sum);
}

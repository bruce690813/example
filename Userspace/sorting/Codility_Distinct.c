/*
Write a function

int solution(int A[], int N);

that, given a zero-indexed array A consisting of N integers, returns the number of distinct values in array A.

Assume that:

N is an integer within the range [0..100,000];
each element of array A is an integer within the range [−1,000,000..1,000,000].
For example, given array A consisting of six elements such that:

 A[0] = 2    A[1] = 1    A[2] = 1
 A[3] = 2    A[4] = 3    A[5] = 1
the function should return 3, because there are 3 distinct values appearing in array A, namely 1, 2 and 3.

Complexity:

expected worst-case time complexity is O(N*log(N));
expected worst-case space complexity is O(N), beyond input storage (not counting the storage required for input arguments).
Elements of input arrays can be modified.

Detected time complexity:
O(N*log(N)) or O(N)
expand allExample tests
▶ example1
example test, positive answer ✔OK
expand allCorrectness tests
▶ extreme_empty
empty sequence ✔OK
▶ extreme_single
sequence of one element ✔OK
▶ extreme_two_elems
sequence of three distinct elements ✔OK
▶ extreme_one_value
sequence of 10 equal elements ✔OK
▶ extreme_negative
sequence of negative elements, length=5 ✔OK
▶ extreme_big_values
sequence with big values, length=5 ✔OK
▶ medium1
chaotic sequence of value sfrom [0..1K], length=100 ✔OK
▶ medium2
chaotic sequence of value sfrom [0..1K], length=200 ✔OK
▶ medium3
chaotic sequence of values from [0..10], length=200 ✔OK
expand allPerformance tests
▶ large1
chaotic sequence of values from [0..100K], length=10K ✔OK
▶ large_random1
chaotic sequence of values from [-1M..1M], length=100K ✔OK
▶ large_random2
another chaotic sequence of values from [-1M..1M], length=100K ✔OK
*/

int cmpfunc(const void *a, const void *b)
{
    return ( *(int*)a - *(int*)b );
}

int solution(int A[], int N)
{
    if (N == 0) return 0;
    int i;
    int distinct_count = 1;

    //quick sort
    qsort(A, N, sizeof(int), cmpfunc);

    //xor
    for (i = 0; i < N; i++) {
        if (i == N-1) break;
        if ((A[i] ^ A[i+1]) != 0)  distinct_count++;
    }

    return distinct_count;
}

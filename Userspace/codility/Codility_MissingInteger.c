/*
This is a demo task.
MissingInteger: Find the smallest positive integer that does not occur in a given sequence.

Write a function:

int solution(int A[], int N);

that, given an array A of N integers, returns the smallest positive integer (greater than 0) that does not occur in A.

For example, given A = [1, 3, 6, 4, 1, 2], the function should return 5.

Given A = [1, 2, 3], the function should return 4.
Given A = [−1, −3], the function should return 1.
Given A = [100, 101, 102, 103, 104], the function should return 1.
For example, for the input [4, 5, 6, 2] (expected 1).

Assume that:

N is an integer within the range [1..100,000];
each element of array A is an integer within the range [−1,000,000..1,000,000].
Complexity:

expected worst-case time complexity is O(N);
expected worst-case space complexity is O(N), beyond input storage (not counting the storage required for input arguments).
Elements of input arrays can be modified.

Detected time complexity:
O(N) or O(N * log(N))
expand allExample tests
▶ example1
first example test ✔OK
▶ example2
second example test ✔OK
▶ example3
third example test ✔OK
expand allCorrectness tests
▶ extreme_single
a single element ✔OK
▶ simple
simple test ✔OK
▶ extreme_min_max_value
minimal and maximal values ✔OK
▶ positive_only
shuffled sequence of 0...100 and then 102...200 ✔OK
▶ negative_only
shuffled sequence -100 ... -1 ✔OK
expand allPerformance tests
▶ medium
chaotic sequences length=10005 (with minus) ✔OK
▶ large_1
chaotic + sequence 1, 2, ..., 40000 (without minus) ✔OK
▶ large_2
shuffled sequence 1, 2, ..., 100000 (without minus) ✔OK
▶ large_3
chaotic + many -1, 1, 2, 3 (with minus) ✔OK
*/

int solution(int A[], int N)
{
    int *s = (int *)malloc(N*sizeof(int));
    int i;
    for (i = 0; i < N; i++) {
        if (A[i] < 1 || A[i] > N) {
            continue;
        } else {
            s[A[i]-1] = A[i];
        }
    }

    for (i = 0; i < N; i++) {
        if (s[i] != i+1) {
            return i+1;
        }
    }

    return N+1;
}

/*
88% For example, for the input [4, 5, 6, 2] the solution returned a wrong answer (got 3 expected 1).

int cmpfunc(const void *a, const void *b)
{
    return ( *(int*)a - *(int*)b );
}

int solution(int A[], int N)
{
    //quick sort
    qsort(A, N, sizeof(int), cmpfunc);

    int i;
    int smallest_positive = 0;

    for (i = 0; i < N; i++) {
        if (A[i] <= 0 || A[i] > N) continue;
        if ((i != N-1) && A[i+1] == A[i]) continue;

        if (A[i+1] != A[i]+1)
            return A[i]+1;
    }

    return smallest_positive+1;
}
*/



/*
Triangle: Determine whether a triangle can be built from a given set of edges.

A zero-indexed array A consisting of N integers is given. A triplet (P, Q, R) is triangular if 0 ≤ P < Q < R < N and:

A[P] + A[Q] > A[R],
A[Q] + A[R] > A[P],
A[R] + A[P] > A[Q].
For example, consider array A such that:

  A[0] = 10    A[1] = 2    A[2] = 5
  A[3] = 1     A[4] = 8    A[5] = 20
Triplet (0, 2, 4) is triangular.

Write a function:

int solution(int A[], int N);

that, given a zero-indexed array A consisting of N integers, returns 1 if there exists a triangular triplet for this array and returns 0 otherwise.

For example, given array A such that:

  A[0] = 10    A[1] = 2    A[2] = 5
  A[3] = 1     A[4] = 8    A[5] = 20
the function should return 1, as explained above. Given array A such that:

  A[0] = 10    A[1] = 50    A[2] = 5
  A[3] = 1
the function should return 0.

For example, for the input [5, 3, 3] (expected 1).
For example, for the input [1, 1, 1, 1, 5, 5, 5] (expected 1).

Assume that:
N is an integer within the range [0..100,000];
each element of array A is an integer within the range [−2,147,483,648..2,147,483,647].

Complexity:
expected worst-case time complexity is O(N*log(N));
expected worst-case space complexity is O(N), beyond input storage (not counting the storage required for input arguments).
Elements of input arrays can be modified.

Detected time complexity:
O(N*log(N))
expand allExample tests
▶ example
example, positive answer, length=6 ✔OK
▶ example1
example, answer is zero, length=4 ✔OK
expand allCorrectness tests
▶ extreme_empty
empty sequence ✔OK
▶ extreme_single
1-element sequence ✔OK
▶ extreme_two_elems
2-element sequence ✔OK
▶ extreme_negative1
three equal negative numbers ✔OK
▶ extreme_arith_overflow1
overflow test, 3 MAXINTs ✔OK
▶ extreme_arith_overflow2
overflow test, 10 and 2 MININTs ✔OK
▶ extreme_arith_overflow3
overflow test, 0 and 2 MAXINTs ✔OK
▶ medium1
chaotic sequence of values from [0..100K], length=30 ✔OK
▶ medium2
chaotic sequence of values from [0..1K], length=50 ✔OK
▶ medium3
chaotic sequence of values from [0..1K], length=100 ✔OK
expand allPerformance tests
▶ large1
chaotic sequence with values from [0..100K], length=10K ✔OK
▶ large2
1 followed by an ascending sequence of ~50K elements from [0..100K], length=~50K ✔OK
▶ large_random
chaotic sequence of values from [0..1M], length=100K ✔OK
▶ large_negative
chaotic sequence of negative values from [-1M..-1], length=100K ✔OK
▶ large_negative2
chaotic sequence of negative values from [-10..-1], length=100K ✔OK
▶ large_negative3
sequence of -1 value, length=100K ✔OK
*/

int cmpfunc(const void *a, const void *b)
{
    return ( *(int*)a - *(int*)b );
}

int solution(int A[], int N)
{
    if (N < 3) return 0;
    int i;

    //quick sort
    qsort(A, N, sizeof(int), cmpfunc);

    for (i = 0; i < N-2; i++) {
        long long P, Q, R;
        P = A[i];
        Q = A[i+1];
        R = A[i+2];
        if (((P + Q) > R) && (((Q + R) > P) && ((P + R) > Q)))
            return 1;
    }

    return 0;
}

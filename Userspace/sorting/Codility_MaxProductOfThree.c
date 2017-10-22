/*
A non-empty zero-indexed array A consisting of N integers is given. The product of triplet (P, Q, R) equates to A[P] * A[Q] * A[R] (0 ≤ P < Q < R < N).

For example, array A such that:

  A[0] = -3
  A[1] = 1
  A[2] = 2
  A[3] = -2
  A[4] = 5
  A[5] = 6
contains the following example triplets:

(0, 1, 2), product is −3 * 1 * 2 = −6
(1, 2, 4), product is 1 * 2 * 5 = 10
(2, 4, 5), product is 2 * 5 * 6 = 60
Your goal is to find the maximal product of any triplet.

Write a function:

int solution(int A[], int N);

that, given a non-empty zero-indexed array A, returns the value of the maximal product of any triplet.

For example, given array A such that:

  A[0] = -3
  A[1] = 1
  A[2] = 2
  A[3] = -2
  A[4] = 5
  A[5] = 6
the function should return 60, as the product of triplet (2, 4, 5) is maximal.

For example, for the input [-10, -2, -4] (expected -80).
For example, for the input [-5, 5, -5, 4] (expected 125).

Assume that:

N is an integer within the range [3..100,000];
each element of array A is an integer within the range [−1,000..1,000].

Complexity:

expected worst-case time complexity is O(N*log(N));
expected worst-case space complexity is O(1), beyond input storage (not counting the storage required for input arguments).
Elements of input arrays can be modified.

Detected time complexity:
O(N * log(N))
expand allExample tests
▶ example 
example test ✔OK
expand allCorrectness tests
▶ one_triple 
three elements ✔OK
▶ simple1 
simple tests ✔OK
▶ simple2 
simple tests ✔OK
▶ small_random 
random small, length = 100 ✔OK
expand allPerformance tests
▶ medium_range 
-1000, -999, ... 1000, length = ~1,000 ✔OK
▶ medium_random 
random medium, length = ~10,000 ✔OK
▶ large_random 
random large, length = ~100,000 ✔OK
▶ large_range 
2000 * (-10..10) + [-1000, 500, -1] ✔OK
▶ extreme_large 
(-2, .., -2, 1, .., 1) and (MAX_INT)..(MAX_INT), length = ~100,000 ✔OK
*/

int cmpfunc(const void *a, const void *b)
{
    return ( *(int*)a - *(int*)b );
}

int solution(int A[], int N)
{
    //quick sort
    qsort(A, N, sizeof(int), cmpfunc);

    long left = A[0]*A[1]*A[N-1];
    long right = A[N-1]*A[N-2]*A[N-3];
    return left > right ? left : right;
}

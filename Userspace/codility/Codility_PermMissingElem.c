/*
PermMissingElem: Find the missing element in a given permutation.

A zero-indexed array A consisting of N different integers is given. The array contains integers in the range [1..(N + 1)], which means that exactly one element is missing.

Your goal is to find that missing element.

Write a function:

int solution(int A[], int N);
that, given a zero-indexed array A, returns the value of the missing element.

For example, given array A such that:

  A[0] = 2
  A[1] = 3
  A[2] = 1
  A[3] = 5
the function should return 4, as it is the missing element.

Assume that:
N is an integer within the range [0..100,000];
the elements of A are all distinct;
each element of array A is an integer within the range [1..(N + 1)].

Complexity:
expected worst-case time complexity is O(N);
expected worst-case space complexity is O(1), beyond input storage (not counting the storage required for input arguments).
Elements of input arrays can be modified.

Detected time complexity:
O(N) or O(N * log(N))
expand allExample tests
▶ example
example test ✔OK
expand allCorrectness tests
▶ empty_and_single
empty list and single element ✔OK
▶ missing_first_or_last
the first or the last element is missing ✔OK
▶ single
single element ✔OK
▶ double
two elements ✔OK
▶ simple
simple test ✔OK
expand allPerformance tests
▶ medium1
medium test, length = ~10,000 ✔OK
▶ medium2
medium test, length = ~10,000 ✔OK
▶ large_range
range sequence, length = ~100,000 ✔OK
▶ large1
large test, length = ~100,000 ✔OK
▶ large2
large test, length = ~100,000 ✔OK
*/

int solution(int A[], int N)
{
    int i;
    long long len = N+1;
    long long sum = (len*(len+1))/2;
    //long long sum = ((1+(N+1))*(N+1))/2;

    for (i = 0; i < N; i++)
        sum -= (long)A[i];

    return sum;
}

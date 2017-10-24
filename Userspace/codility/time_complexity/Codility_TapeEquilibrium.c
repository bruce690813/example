/*
TapeEquilibrium: Minimize the value |(A[0] + ... + A[P-1]) - (A[P] + ... + A[N-1])|.

A non-empty zero-indexed array A consisting of N integers is given. Array A represents numbers on a tape.

Any integer P, such that 0 < P < N, splits this tape into two non-empty parts: A[0], A[1], ..., A[P − 1] and A[P], A[P + 1], ..., A[N − 1].

The difference between the two parts is the value of: |(A[0] + A[1] + ... + A[P − 1]) − (A[P] + A[P + 1] + ... + A[N − 1])|

In other words, it is the absolute difference between the sum of the first part and the sum of the second part.

For example, consider array A such that:

  A[0] = 3
  A[1] = 1
  A[2] = 2
  A[3] = 4
  A[4] = 3
We can split this tape in four places:

P = 1, difference = |3 − 10| = 7
P = 2, difference = |4 − 9| = 5
P = 3, difference = |6 − 7| = 1
P = 4, difference = |10 − 3| = 7
Write a function:

int solution(int A[], int N);

that, given a non-empty zero-indexed array A of N integers, returns the minimal difference that can be achieved.

For example, given:

  A[0] = 3
  A[1] = 1
  A[2] = 2
  A[3] = 4
  A[4] = 3
the function should return 1, as explained above.

Assume that:
N is an integer within the range [2..100,000];
each element of array A is an integer within the range [−1,000..1,000].

Complexity:
expected worst-case time complexity is O(N);
expected worst-case space complexity is O(N), beyond input storage (not counting the storage required for input arguments).
Elements of input arrays can be modified.

Detected time complexity:
O(N)
expand allExample tests
▶ example
example test ✔OK
expand allCorrectness tests
▶ double
two elements ✔OK
▶ simple_positive
simple test with positive numbers, length = 5 ✔OK
▶ simple_negative
simple test with negative numbers, length = 5 ✔OK
▶ small_random
random small, length = 100 ✔OK
▶ small_range
range sequence, length = ~1,000 ✔OK
▶ small
small elements ✔OK
expand allPerformance tests
▶ medium_random1
random medium, numbers from 0 to 100, length = ~10,000 ✔OK
▶ medium_random2
random medium, numbers from -1,000 to 50, length = ~10,000 ✔OK
▶ large_ones
large sequence, numbers from -1 to 1, length = ~100,000 ✔OK
▶ large_random
random large, length = ~100,000 ✔OK
▶ large_sequence
large sequence, length = ~100,000 ✔OK
▶ large_extreme
large test with maximal and minimal values, length = ~100,000 ✔OK
*/

#define min(a, b) (a > b) ? b : a

int solution(int A[], int N)
{
    int i;
    int min_diff = 0, second_sum = 0;

    int first_sum = A[0];
    for (i = 1; i < N; i++) {
        second_sum+=A[i];
    }

    min_diff = abs(first_sum - second_sum);

    for (i = 1; i < N-2; i++) {
        first_sum+=A[i];
        second_sum-=A[i];
        int diff = abs(first_sum - second_sum);
        min_diff = min(diff, min_diff);
    }

    return min_diff;
}

/*
A positive integer D is a factor of a positive integer N if there exists an integer M such that N = D * M.

For example, 6 is a factor of 24, because M = 4 satisfies the above condition (24 = 6 * 4).

Write a function:

int solution(int N);

that, given a positive integer N, returns the number of its factors.

For example, given N = 24, the function should return 8, because 24 has 8 factors, namely 1, 2, 3, 4, 6, 8, 12, 24. There are no other factors of 24.

Assume that:
N is an integer within the range [1..2,147,483,647].

Complexity:
expected worst-case time complexity is O(sqrt(N));
expected worst-case space complexity is O(1).

Detected time complexity:
O(sqrt(N))
expand allExample tests
▶ example1
example test (N=24=4!) ✔OK
expand allCorrectness tests
▶ squares
N=16, N=36 ✔OK
▶ tiny
N <= 10 ✔OK
▶ simple1
N=41(prime), N=42 ✔OK
▶ simple2
N=69, N=64, N=120=5! ✔OK
▶ simple3
N=720=6!, N=1111 ✔OK
▶ simple4
N=5,040=7!, N=12,345 ✔OK
▶ simple5
N=34,879, N=40,320=8! ✔OK
▶ extreme_one
N=1 ✔OK
expand allPerformance tests
▶ medium1
N=362,880=9!, N=1,948,102 ✔OK
▶ medium2
N=3,628,800=10!, N=5,621,892, N=4,999,696 ✔OK
▶ big1
N=27,043,111, N=39,916,800=11!, N = 39,992,976 ✔OK
▶ big2
N=97,093,212, N=2^28 ✔OK
▶ big3
N=479,001,600=12!, N=780291637(prime), N=449,991,369 ✔OK
▶ extreme_maxint
N=1,000,000,000, N=MAX_INT, N=2147,395,600 ✔OK
*/

#include <math.h>

int solution(int N)
{
    int i;
    int factor_num = 0;

    for (i = 1; i <= sqrt(N); i++) {
        if ((N%i) == 0) {
            if ((i*i) != N)
                factor_num+=2;
            else if ((i*i) == N)
                factor_num++;
        }
    }

    return factor_num;
}

/*
MinPerimeterRectangle: Find the minimal perimeter of any rectangle whose area equals N.

An integer N is given, representing the area of some rectangle.

The area of a rectangle whose sides are of length A and B is A * B, and the perimeter is 2 * (A + B).

The goal is to find the minimal perimeter of any rectangle whose area equals N. The sides of this rectangle should be only integers.

For example, given integer N = 30, rectangles of area 30 are:

(1, 30), with a perimeter of 62,
(2, 15), with a perimeter of 34,
(3, 10), with a perimeter of 26,
(5, 6), with a perimeter of 22.
Write a function:

int solution(int N);

that, given an integer N, returns the minimal perimeter of any rectangle whose area is exactly equal to N.

For example, given an integer N = 30, the function should return 22, as explained above.

For example, for the input 100000000 the solution exceeded the time limit.

Assume that:
N is an integer within the range [1..1,000,000,000].

Complexity:
expected worst-case time complexity is O(sqrt(N));
expected worst-case space complexity is O(1).

Detected time complexity:
O(sqrt(N))
expand allExample tests
▶ example
example test ✔OK
expand allCorrectness tests
▶ extreme_min
N = 1 test ✔OK
▶ simple1
N = 36 test ✔OK
▶ simple2
N = 48 test ✔OK
▶ simple3
N = 101 test ✔OK
▶ small
N = 1,234 test ✔OK
expand allPerformance tests
▶ medium
N = 4,564,320 test ✔OK
▶ prime1
N = 15,486,451 test ✔OK
▶ square
N = 100,000,000 test ✔OK
▶ prime2
N = 982,451,653 test ✔OK
▶ extreme_max
N = 1,000,000,000 test ✔OK
*/

#include <math.h>

int solution(int N)
{
    int i;
    int min_perimeter = 2*(1+N);

    for (i = 1; i <= (int)sqrt(N); i++) {
        if (N%i == 0) {
            int perimeter = 2*(i+N/i);
            min_perimeter = (perimeter < min_perimeter) ? perimeter : min_perimeter;
        }
    }

    return min_perimeter;
}


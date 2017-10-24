/*
FrogJmp: Count minimal number of jumps from position X to Y.

A small frog wants to get to the other side of the road. The frog is currently located at position X and wants to get to a position greater than or equal to Y. The small frog always jumps a fixed distance, D.

Count the minimal number of jumps that the small frog must perform to reach its target.

Write a function:

int solution(int X, int Y, int D);
that, given three integers X, Y and D, returns the minimal number of jumps from position X to a position equal to or greater than Y.

For example, given:

  X = 10
  Y = 85
  D = 30
the function should return 3, because the frog will be positioned as follows:

after the first jump, at position 10 + 30 = 40
after the second jump, at position 10 + 30 + 30 = 70
after the third jump, at position 10 + 30 + 30 + 30 = 100

For example, for the input (5, 1000000000, 2) the solution exceeded the time limit.
Example test: (10, 85, 30) (expected 3)
For example, for the input (1, 5, 2) (expected 2).

Assume that:
X, Y and D are integers within the range [1..1,000,000,000];
X ≤ Y.

Complexity:
expected worst-case time complexity is O(1);
expected worst-case space complexity is O(1).

Detected time complexity:
O(1)
expand allExample tests
▶ example
example test ✔OK
expand allCorrectness tests
▶ simple1
simple test ✔OK
▶ simple2 ✔OK
▶ extreme_position
no jump needed ✔OK
▶ small_extreme_jump
one big jump ✔OK
expand allPerformance tests
▶ many_jump1
many jumps, D = 2 ✔OK
▶ many_jump2
many jumps, D = 99 ✔OK
▶ many_jump3
many jumps, D = 1283 ✔OK
▶ big_extreme_jump
maximal number of jumps ✔OK
▶ small_jumps
many small jumps ✔OK
*/

int solution(int X, int Y, int D)
{
    if (X < Y) {
        int quotient = (Y - X)/D;
        int remainder = (Y - X)%D;
        return (remainder > 0) ? quotient+1 : quotient;
    }

    return 0;
}

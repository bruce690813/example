/*
A zero-indexed array A consisting of N integers is given. An inversion is a pair of indexes (P, Q) such that P < Q and A[Q] < A[P].

Write a function:

int solution(int A[], int N);

that computes the number of inversions in A, or returns −1 if it exceeds 1,000,000,000.

Assume that:
N is an integer within the range [0..100,000];
each element of array A is an integer within the range [−2,147,483,648..2,147,483,647].
For example, in the following array:

 A[0] = -1 A[1] = 6 A[2] = 3
 A[3] =  4 A[4] = 7 A[5] = 4
there are four inversions:

   (1,2)  (1,3)  (1,5)  (4,5)
so the function should return 4.

Complexity:
expected worst-case time complexity is O(N*log(N));
expected worst-case space complexity is O(N), beyond input storage (not counting the storage required for input arguments).
Elements of input arrays can be modified.
*/

/*
The following issues have been detected: timeout errors.
Correctness: 100%, Performance: 40% = Task score: 72%

Detected time complexity: O(N**2)

Performance tests
▶ big2
n=20000 ✘TIMEOUT ERROR
running time: 0.13 sec., time limit: 0.10 sec.
▶ big3
n=30000 ✘TIMEOUT ERROR
running time: 0.32 sec., time limit: 0.10 sec.
▶ big_monotonic
long descending and non-ascending sequence ✘TIMEOUT ERROR
running time: 0.65 sec., time limit: 0.10 sec.

A[ ] = {-1, 6, 3, 4, 7, 4}
 0 1 2 3 4 5
------------
-1 6 3 4 7 4
   6 3 4 7 4  --> (1,2), (1,3), (1,5)
     3 4 7 4
       4 7 4
         7 4  --> (4,5)
*/

int solution(int A[], int N)
{
    int i, j;
    int inversion_num = 0;

    for (i = 0; i < N; i++) {
        for (j = i+1; j < N; j++) {
            if (A[i] > A[j]) inversion_num++;
        }
    }

    if (inversion_num > 1000000000) return -1;

    return inversion_num;
}

int  _mergeSort(int arr[], int temp[], int left, int right);
int merge(int arr[], int temp[], int left, int mid, int right);

/* This function sorts the input array and returns the
   number of inversions in the array */
int solution(int A[], int N)
{
    int *temp = (int *)malloc(sizeof(int)*N);
    return _mergeSort(A, temp, 0, N - 1);
}

/* An auxiliary recursive function that sorts the input array and
  returns the number of inversions in the array. */
int _mergeSort(int arr[], int temp[], int left, int right)
{
    int mid, inv_count = 0;
    if (right > left) {
        /* Divide the array into two parts and call _mergeSortAndCountInv()
           for each of the parts */
        mid = (right + left)/2;

        /* Inversion count will be sum of inversions in left-part, right-part
          and number of inversions in merging */
        inv_count  = _mergeSort(arr, temp, left, mid);
        inv_count += _mergeSort(arr, temp, mid+1, right);

        /*Merge the two parts*/
        inv_count += merge(arr, temp, left, mid+1, right);
    }

    if (inv_count > 1000000000) return -1;

    return inv_count;
}

/* This funt merges two sorted arrays and returns inversion count in
   the arrays.*/
int merge(int arr[], int temp[], int left, int mid, int right)
{
    int i, j, k;
    int inv_count = 0;

    i = left; /* i is index for left subarray*/
    j = mid;  /* j is index for right subarray*/
    k = left; /* k is index for resultant merged subarray*/
    while ((i <= mid - 1) && (j <= right)) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];

            /*this is tricky -- see above explanation/diagram for merge()*/
            inv_count = inv_count + (mid - i);
        }
    }

    /* Copy the remaining elements of left subarray
     (if there are any) to temp*/
    while (i <= mid - 1)
        temp[k++] = arr[i++];

    /* Copy the remaining elements of right subarray
     (if there are any) to temp*/
    while (j <= right)
        temp[k++] = arr[j++];

    /*Copy back the merged elements to original array*/
    for (i=left; i <= right; i++)
        arr[i] = temp[i];

    return inv_count;
}

void main()
{
    int arr[] = {1, 20, 6, 4, 5};
    printf(" Number of inversions are %d \n", mergeSort(arr, 5));
    return 0;
}

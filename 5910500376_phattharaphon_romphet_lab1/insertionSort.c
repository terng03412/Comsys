#include <stdio.h>
#define N 12
void insertionSortRecursive(int arr[], int n)
{
    int last, j;
    if (n <= 1)
        return;
    insertionSortRecursive(arr, n - 1);
    last = arr[n - 1];
    j = n - 2;
    while (j >= 0 && arr[j] > last)
    {
        arr[j + 1] = arr[j];
        j--;
    }
    arr[j + 1] = last;
}
void printArray(int arr[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}
int main()
{
    int data[] = {132470, 324545, 73245, 93245, 80324542, 244, 2,
                  66, 236, 327, 236, 21544};
    printArray(data, N);
    insertionSortRecursive(data, N);
    printArray(data, N);
    return 0;
}
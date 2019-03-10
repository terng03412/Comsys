#include <stdio.h>

int main()
{
    int A[20];
    int i;
    for (i = 0; i < 20; i++)
    {
        if (i % 2 == 0)
            A[i] = 1;
        else
            A[i] = 0;
    }
    return 0;
}
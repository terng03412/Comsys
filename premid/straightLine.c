#include <stdio.h>

int main()
{
    int a = 10;
    int x = 10;
    a = a >> 31;
    int y = a << 1;
    // printf("%d\n", a);
    printf("%d\n", x - 1 + (a * y));
    return 0;
}
#include <stdio.h>

int main()
{
    int a = -10;
    int x = 10;

    printf("a: %d\n", a);
    a = a >> 31;
    printf("a>>31 %d\n", a);

    int y = a << 1;
    printf("a<<1 %d\n", y);
    // printf("%d\n", a);
    printf("x = %d\n", x - 1 + (a * y));
    return 0;
}
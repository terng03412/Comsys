#include <stdio.h>
#define CHAR_BIT 8

int bitAnd(int x, int y)
{
    return ~(~x | ~y);
    // !(a|b) = !a&!b
    // a&b = !(!a|!b)
}

int getByte(int x, int n)
{
    return 0xff & (x >> (n << 3));
    // Convert bits to byte by 3 shift left and filter
}

int logicalShift(int x, int n)
{
    int test = x >> 31;
    int fix = 1 << (33 + ~n);
    return (~test & (x >> n)) | (test & ((x >> n) + fix));
    // get unsigned 0xffffffff test , shift right
    // eliminate SRA effected bit
}

int bitCount(int x)
{
    int mask = 0x11 + (0x11 << 8) + (0x11 << 16) + (0x11 << 24);
    int count = x & mask;
    int total = 0;
    int addMask = 0x0F;
    x = x >> 1;
    count += x & mask;
    x = x >> 1;
    count += x & mask;
    x = x >> 1;
    count += x & mask;
    total += (count & addMask) + ((count >> 4) & addMask) + ((count >> 8) & addMask) + ((count >> 12) & addMask) + ((count >> 16) & addMask) + ((count >> 20) & addMask) + ((count >> 24) & addMask) + ((count >> 28) & addMask);
    return total;
    // split in to many bitmask and use pattern in 32 bits
}

int tmin()
{
    int one = 1;
    int ans = one << 31;
    return ans;
    // minimum = 0x111111
}

int odd_ones(unsigned x)
{
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;

    // input = abcde
    // output = (a+b+c+d+e)&1
    return (x & 0x1);
}

int negate(int x)
{
    x = ~x;
    x = x + 1;
    return x;
    // invert the number in two complement system
}

int fitsBits(int x, int n)
{
    int shift = 32 + ~n + 1;
    return !(x ^ ((x << shift) >> shift));
    //shift with remain bit to classifie x will be loss or not
}

int isPositive(int x)
{
    return !((x & (1 << 31)) | !x);
    // get 0xffffffff to compare with x and check sign bit
}

int isLessOrEqual(int x, int y)
{
    return !(((y + (~x + 1)) >> 31) & 1);
    // invert y + -x and shift 31 and check sign bit
}

int main()
{
    printf("\n\n ------- BitAnd------- \n\n");
    printf("bitAnd(1, 1) = %d\n", bitAnd(1, 1));
    printf("bitAnd(0, 1) = %d\n", bitAnd(0, 1));
    printf("bitAnd(255, 0) = %d\n", bitAnd(255, 0));
    printf("bitAnd(255, 255) = %d\n", bitAnd(255, 255));
    printf("bitAnd(0x10101010, 0x11111111) = %d\n", bitAnd(0x10101010, 0x11111111));
    printf("bitAnd(0x11111111, 0x11111111) = 0x%x\n", bitAnd(0x11111111, 0x11111111));

    printf("\n\n ------- GetByte------- \n\n");
    printf("getByte(0x12345678, 0) = 0x%x\n", getByte(0x12345678, 0));
    printf("getByte(0x12345678, 1) = 0x%x\n", getByte(0x12345678, 1));
    printf("getByte(0x12345678, 2) = 0x%x\n", getByte(0x12345678, 2));
    printf("getByte(0x12345678, 3) = 0x%x\n", getByte(0x12345678, 3));
    printf("getByte(0x12345678, 4) = 0x%x\n", getByte(0x12345678, 4));
    printf("getByte(0x12345678, 5) = 0x%x\n", getByte(0x12345678, 5));

    printf("\n\n ------- logicalShift------- \n\n");

    printf("logicalShift(0x87654321,0) = 0x%x\n", logicalShift(0x87654321, 0));
    printf("logicalShift(0x87654321,1) = 0x%x\n", logicalShift(0x87654321, 1));
    printf("logicalShift(0x87654321,2) = 0x%x\n", logicalShift(0x87654321, 2));
    printf("logicalShift(0x87654321,3) = 0x%x\n", logicalShift(0x87654321, 3));
    printf("logicalShift(0x87654321,4) = 0x%x\n", logicalShift(0x87654321, 4));
    printf("logicalShift(0x87654321,5) = 0x%x\n", logicalShift(0x87654321, 5));

    printf("\n\n ------- bitCount------- \n\n");
    printf("bitCount(5) = %d\n", bitCount(5));
    printf("bitCount(7) = %d\n", bitCount(7));
    printf("bitCount(9) = %d\n", bitCount(9));
    printf("bitCount(10) = %d\n", bitCount(10));
    printf("bitCount(0) = %d\n", bitCount(0));
    printf("bitCount(255) = %d\n", bitCount(255));

    printf("\n\n ------- odd_ones------- \n\n");
    printf("odd_ones(4) = %d\n", odd_ones(4));
    printf("odd_ones(5) = %d\n", odd_ones(5));
    printf("odd_ones(255) = %d\n", odd_ones(255));
    printf("odd_ones(0) = %d\n", odd_ones(0));
    printf("odd_ones(6) = %d\n", odd_ones(6));
    printf("odd_ones(-1) = %d\n", odd_ones(-1));

    printf("\n\n ------- tmin------- \n\n");
    printf("tmin() = %d\n", tmin());

    printf("\n\n ------- fitsBits------- \n\n");
    printf("fitsBits(5,3) = %d\n", fitsBits(5, 3));
    printf("fitsBits(-4,3) = %d\n", fitsBits(-4, 3));
    printf("fitsBits(10,4) = %d\n", fitsBits(10, 4));
    printf("fitsBits(12,4) = %d\n", fitsBits(12, 4));
    printf("fitsBits(255,1) = %d\n", fitsBits(255, 1));
    printf("fitsBits(255,8) = %d\n", fitsBits(255, 8));

    printf("\n\n ------- negate------- \n\n");
    printf("negate(1) = %d\n", negate(1));
    printf("negate(-1) = %d\n", negate(-1));
    printf("negate(255) = %d\n", negate(255));
    printf("negate(-255) = %d\n", negate(-255));
    printf("negate(10) = %d\n", negate(10));
    printf("negate(-10) = %d\n", negate(-10));

    printf("\n\n ------- isPositive------- \n\n");
    printf("isPositive(-1) = %d\n", isPositive(-1));
    printf("isPositive(1) = %d\n", isPositive(1));
    printf("isPositive(255) = %d\n", isPositive(255));
    printf("isPositive(-255) = %d\n", isPositive(-255));
    printf("isPositive(-10) = %d\n", isPositive(-10));
    printf("isPositive(10) = %d\n", isPositive(10));
    printf("isPositive(-100) = %d\n", isPositive(-100));

    printf("\n\n ------- isLessOrEqual------- \n\n");
    printf("isLessOrEqual(4,5) = %d\n", isLessOrEqual(4, 5));
    printf("isLessOrEqual(5,5) = %d\n", isLessOrEqual(5, 5));
    printf("isLessOrEqual(6,5) = %d\n", isLessOrEqual(6, 5));
    printf("isLessOrEqual(10,5) = %d\n", isLessOrEqual(10, 5));
    printf("isLessOrEqual(255,255) = %d\n", isLessOrEqual(255, 255));
    printf("isLessOrEqual(0,-1) = %d\n", isLessOrEqual(0, -1));

    return 0;
}

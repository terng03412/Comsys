#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
static FILE *f;
static int ch;
static unsigned int val;
enum
{
    plus,
    minus,
    times,
    divide,
    mod,
    lparen,
    rparen,
    number,
    eof,
    illegal
};
static void SInit(char *filename)
{
    ch = EOF;
    f = fopen(filename, "r+t");
    if (f != NULL)
        ch = getc(f);
}
static void Number()
{
    val = 0;
    while (('0' <= ch) && (ch <= '9'))
    {
        val = val * 10 + ch - '0';
        ch = getc(f);
    }

    printf("li $a0 %d \nsw $a0, 0($sp) \naddi $sp, $sp, -4\n", val);
}
static int SGet()
{
    register int sym;
    while ((ch != EOF) && (ch <= ' '))
        ch = getc(f);

    switch (ch)
    {
    case EOF:
        sym = eof;
        break;
    case '+':
        sym = plus;
        ch = getc(f);
        break;
    case '-':
        sym = minus;
        ch = getc(f);
        break;
    case '*':
        sym = times;
        ch = getc(f);
        break;
    case '/':
        sym = divide;
        ch = getc(f);
        break;
    case '%':
        sym = mod;
        ch = getc(f);
        break;
    case '(':
        sym = lparen;
        ch = getc(f);
        break;
    case ')':
        sym = rparen;
        ch = getc(f);
        break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        sym = number;

        Number();
        break;

    default:
        sym = illegal;
    }

    return sym;
}
static int sym;
static int Expr();
static int Factor()
{
    int res;
    assert((sym == number) || (sym == lparen));
    if (sym == number)
    {
        sym = SGet();
        res = val;
    }
    else
    {
        sym = SGet();
        res = Expr();
        assert(sym == rparen);
        sym = SGet();
    }
    return res;
}
static int Term()
{
    int res;
    int par1 = Factor();

    // printf("li $a0 %d \nsw $a0, 0($sp) \naddi $sp, $sp, -4\n", par1);

    res = par1;
    while ((sym == times) || (sym == divide) || (sym == mod))
    {
        int temp = sym;
        sym = SGet();
        int par2 = Factor();

        // printf("li $a0 %d \nsw $a0, 0($sp) \naddi $sp, $sp, -4\n", par2);
        if (temp == times)
        {
            res = par1 * par2;

            printf("lw $t1, 4($sp) \nmul $a0, $a0, $t1 \naddi $sp, $sp, 4\n");
        }
        else if (temp == divide)
        {
            res = par1 / par2;

            printf("lw $t1, 4($sp) \ndiv $a0, $a0, $t1 \naddi $sp, $sp, 4\n");
        }
        else if (temp == mod)
        {
            res = par1 % par2;

            printf("lw $t1, 4($sp) \nmod $a0, $a0, $t1 \naddi $sp, $sp, 4\n");
        }
        par1 = res;
    }
    return res;
}
static int Expr()
{
    if ((sym == minus) || (sym == plus))
        sym = SGet();
    int res;
    int par1 = Term();

    res = par1;
    while ((sym == plus) || (sym == minus))
    {
        int temp = sym;
        sym = SGet();
        int par2 = Term();

        if (temp == plus)
        {
            res = par1 + par2;

            printf("lw $t1, 4($sp) \nadd $a0, $a0, $t1 \naddi $sp, $sp, 4\n");
        }
        else if (temp == minus)
        {
            res = par1 - par2;

            printf("lw $t1, 4($sp) \nsub $a0, $a0, $t1 \naddi $sp, $sp, 4\n");
        }
        par1 = res;
    }
    return res;
}
int main(int argc, char *argv[])
{
    printf(".text # text section \n.globl main # call main by SPIM \nmain:\n");
    register int result;
    if (argc == 2)
    {
        SInit(argv[1]);
        sym = SGet();
        int result = Expr();
        assert(sym == eof);
        // printf("result = %d\n", result);
    }
    else
    {
        printf("usage: expreval <filename>\n");
    }
    printf("li   $v0, 1\nsyscall\n");
    printf("end:\nori   $v0, $0, 10  # system call 10 for exit\nsyscall            # we are out of here.\n");
    return 0;
}

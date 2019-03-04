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
    printf("get first char \n");
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
    printf("\n");
}
static int SGet(int l)
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
    printf("\n");
    return sym;
}
static int sym;
static int Expr();
static int Factor(int l, int f)
{
    printf("Factor Lv %d On %d \n\n", l, f);
    int k = l + 1;
    // printf("--------Factor------- \n");
    int res;
    assert((sym == number) || (sym == lparen));
    if (sym == number)
    {
        sym = SGet(l);
        res = val;
    }
    else
    {
        sym = SGet(l);

        printf("From Factor Lv %d On %d ", l, f);
        res = Expr(l + 1, f - 1);

        assert(sym == rparen);
        sym = SGet(l);
    }
    printf("\n\n");
    return res;
}
static int Term(int l, int f)
{
    printf("Term Lv %d On %d \n\n", l, f);

    int res;
    // printf("From Term Lv %d On %d ", l, f);
    int par1 = Factor(l + 1, f - 1);
    res = par1;
    while ((sym == times) || (sym == divide) || (sym == mod))
    {
        int temp = sym;

        sym = SGet(l);

        // printf("From Term Lv %d On %d ", l, f);
        int par2 = Factor(l + 1, f + 1);

        if (temp == times)
        {
            res = par1 * par2;
        }
        else if (temp == divide)
        {
            res = par1 / par2;
        }
        else if (temp == mod)
        {
            res = par1 % par2;
        }
        par1 = res;
    }
    // printf("\n\n");
    return res;
}
static int Expr(int l, int f)
{
    // printf("Expr Lv %d On %d \n", l, f);

    if ((sym == minus) || (sym == plus))
    {
        sym = SGet(l);
        // printf("From Expr sym = +- go to Sget() \n");
    }

    int res;

    printf("From Expr Lv %d On %d ", l, f);
    int par1 = Term(l + 1, f - 1);

    res = par1;
    while ((sym == plus) || (sym == minus))
    {
        int temp = sym;

        // printf("From Expr go to Sget by sym = Sget\n");
        sym = SGet(l);

        printf("From Expr Lv %d On %d ", l, f);
        int par2 = Term(l + 1, f + 1);
        if (temp == plus)
        {
            res = par1 + par2;
        }
        else if (temp == minus)
        {
            res = par1 - par2;
        }
        par1 = res;
    }
    // printf("\n\n");
    return res;
}
int main(int argc, char *argv[])
{
    register int result;
    if (argc == 2)
    {
        SInit(argv[1]);
        sym = SGet(0);
        int result = Expr(0, 0);
        assert(sym == eof);
        printf("result = %d\n", result);
    }
    else
    {
        printf("usage: expreval <filename>\n");
    }
    return 0;
}

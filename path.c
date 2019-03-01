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
    printf("----- Number -----\n");
    printf("get value \n");
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
    printf("------ Sget from %d-----\n", l);
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
static int Factor(int l)
{
    printf("at %d\n", l);
    int k = l + 1;
    printf("--------Factor------- \n");
    int res;
    assert((sym == number) || (sym == lparen));
    if (sym == number)
    {
        printf("Case number of lparen\nfrom Factor go to Sget \n");
        sym = SGet(l);
        res = val;
    }
    else
    {
        printf("Case not number nor lparen\n");
        printf("from Factor go to Sget by sym = Sget\n");
        sym = SGet(l);

        printf("from Factor go to Expr by res = Expr() \n");
        res = Expr(k);

        assert(sym == rparen);
        printf("from Factor go to Sget by sym = Sget \n");
        sym = SGet(l);
    }
    printf("\n\n");
    return res;
}
static int Term(int l)
{
    printf("at %d\n", l);
    int k = l + 1;
    printf("-----Term-----\n");
    int res;
    printf("From Term go to Factor by par1 = Factor() \n");
    int par1 = Factor(k);
    res = par1;
    while ((sym == times) || (sym == divide) || (sym == mod))
    {
        int temp = sym;

        printf("From Term go to Sget\n");
        sym = SGet(l);

        printf("From Term go to Factor by par2 = Factor() \n");
        int par2 = Factor(k);

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
    printf("\n\n");
    return res;
}
static int Expr(int l)
{
    printf("at %d \n\n", l);
    int k = l + 1;

    printf("-----Expr-----\n");
    if ((sym == minus) || (sym == plus))
    {
        sym = SGet(l);
        printf("From Expr sym = +- go to Sget() \n");
    }

    int res;

    printf("From Expr go to Term by par1 = Term \n");
    int par1 = Term(k);

    res = par1;
    while ((sym == plus) || (sym == minus))
    {
        int temp = sym;

        printf("From Expr go to Sget by sym = Sget\n");
        sym = SGet(l);

        printf("From Expr go to Term by par2 = Term\n");
        int par2 = Term(k);
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
    printf("\n\n");
    return res;
}
int main(int argc, char *argv[])
{
    register int result;
    if (argc == 2)
    {
        int l = 0;
        SInit(argv[1]);
        sym = SGet(l);
        int result = Expr(l);
        assert(sym == eof);
        printf("result = %d\n", result);
    }
    else
    {
        printf("usage: expreval <filename>\n");
    }
    return 0;
}

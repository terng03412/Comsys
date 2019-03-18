#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
static FILE *f;
static int ch;
static unsigned int val;
enum
{
    lparen,
    rparen,
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
    case '(':
        sym = lparen;
        ch = getc(f);
        break;
    case ')':
        sym = rparen;
        ch = getc(f);
        break;
    default:
        sym = illegal;
    }
    return sym;
}

static int sym;
static void A()
{
    while (sym == lparen)
    {
        sym = SGet();
        A();
        assert(sym == rparen);
        sym = SGet();
    }
}

int main(int argc, char *argv[])
{
    register int result;
    if (argc == 2)
    {
        SInit(argv[1]);
        sym = SGet();
        A();
        printf("Accept \n");
    }
    else
    {
        printf("usage: Aeval <filename>\n");
    }
    return 0;
}

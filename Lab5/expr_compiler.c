
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
static FILE *f;
static int ch;
static unsigned int val;

typedef struct NodeDesc *Node;
typedef struct NodeDesc
{
    char kind;        // plus, minus, times, divide, number
    int val;          // number: value
    Node left, right; // plus, minus, times, divide: children
} NodeDesc;

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
static Node Expr();

static Node Factor()
{
    register Node result;
    assert((sym == number) || (sym == lparen));
    if (sym == number)
    {
        result = malloc(sizeof(NodeDesc));
        result->kind = sym;
        result->val = val;
        result->left = NULL;
        result->right = NULL;
        sym = SGet();
    }
    else
    {
        sym = SGet();
        result = Expr();
        assert(sym == rparen);
        sym = SGet();
    }
    return result;
}

static Node Term()
{
    register Node root, result;
    register Node par1 = Factor();
    root = par1;
    while ((sym == times) || (sym == divide) || (sym == mod))
    {
        int temp = sym;
        sym = SGet();

        result = malloc(sizeof(NodeDesc));
        result->kind = temp;

        sym = SGet();
        result->left = root;

        register Node par2 = Factor();
        result->right = par2;
        root = result;
    }
    return root;
}

static Node Expr()
{
    register Node result, root;
    register Node par1 = Term();
    root = par1;
    while (sym == plus || sym == minus)
    {
        int temp = sym;
        sym = SGet();

        result = malloc(sizeof(NodeDesc));
        result->kind = temp;

        result->left = root;
        register Node par2 = Term();
        result->right = par2;
        root = result;
    }
    return root;
}

static void PosF(Node root)
{
    if (root != NULL)
    {
        PosF(root->left);
        PosF(root->right);

        switch (root->kind)
        {
        case plus:
            printf("+");
            break;
        case minus:
            printf("-");
            break;
        case times:
            printf("*");
            break;
        case divide:
            printf("/");
            break;
        case mod:
            printf("%%");
            break;
        case number:
            printf("%d", root->val);
            break;
        }
        printf(" ");
    }
}

int main(int argc, char *argv[])
{
    register Node root, DifRoot;

    if (argc == 2)
    {
        SInit(argv[1]);
        sym = SGet();
        root = Expr();
        // assert(sym == eof);

        printf("PostFix: \t");
        PosF(root);
        printf("\n");
    }
    else
    {
        printf("usage: expreval <filename>\n");
    }
    return 0;
}

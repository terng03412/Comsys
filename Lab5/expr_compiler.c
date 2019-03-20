#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static FILE *f;
static int ch;
static unsigned int val;

FILE *fp;

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

static void GenMIPs(Node root, int pos)
{
    if (root != NULL)
    {
        if (root->kind == number && pos == -1)
        {
            fprintf(fp, "li $a0 %d\n", root->val);
            fprintf(fp, "sw $a0, 0($sp)\n");
            fprintf(fp, "addi $sp, $sp, -4\n");

            return;
        }
        else if (root->kind == number && pos == 1)
        {
            fprintf(fp, "li $a0 %d\n", root->val);

            return;
        }
        GenMIPs(root->left, -1);
        GenMIPs(root->right, 1);

        switch (root->kind)
        {
        case plus:
            fprintf(fp, "lw $t1, 4($sp)\n");
            fprintf(fp, "add $a0, $a0, $t1\n");
            fprintf(fp, "addi $sp, $sp, 4\n");

            return;
        case minus:

            fprintf(fp, "lw $t1, 4($sp)\n");
            fprintf(fp, "sub $a0, $a0, $t1\n");
            fprintf(fp, "addi $sp, $sp, 4\n");

            return;
        case times:

            fprintf(fp, "lw $t1, 4($sp)\n");
            fprintf(fp, "mul $a0, $a0, $t1\n");
            fprintf(fp, "addi $sp, $sp, 4\n");

            return;
        case divide:

            fprintf(fp, "lw $t1, 4($sp)\n");
            fprintf(fp, "div $a0, $t1\n");
            fprintf(fp, "mflo $a0\n");
            fprintf(fp, "addi $sp, $sp, 4\n");

            return;
        case mod:

            fprintf(fp, "lw $t1, 4($sp)\n");
            fprintf(fp, "div $a0, $t1\n");
            fprintf(fp, "mfhi $a0\n");
            fprintf(fp, "addi $sp, $sp, 4\n");

            return;
        }
    }
}

static void Print(Node root, int level)
{
    register int i;
    if (root != NULL)
    {
        Print(root->right, level + 1);
        for (i = 0; i < level; i++)
            printf("\t");
        switch (root->kind)
        {
        case plus:
            printf("+\n");
            break;
        case minus:
            printf("-\n");
            break;
        case times:
            printf("*\n");
            break;
        case divide:
            printf("/\n");
            break;
        case mod:
            printf("%%\n");
            break;
        case number:
            printf("%d\n", root->val);
            break;
        }
        Print(root->left, level + 1);
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
    register Node res;
    assert((sym == number) || (sym == lparen));
    if (sym == number)
    {
        // printf("f1 ");
        res = malloc(sizeof(NodeDesc));
        res->kind = number;
        res->val = val;
        res->left = NULL;
        res->right = NULL;

        sym = SGet();
    }
    else
    {
        // printf("f2 ");
        sym = SGet();
        res = Expr();
        assert(sym == rparen);
        sym = SGet();
    }
    return res;
}

static Node Term()
{
    register Node res;
    // printf("T ");
    Node par1 = Factor();
    res = par1;
    while ((sym == times) || (sym == divide) || (sym == mod))
    {
        int temp = sym;
        sym = SGet();
        Node par2 = Factor();
        if (temp == times)
        {
            // printf("tt ");
            res = malloc(sizeof(NodeDesc));
            res->kind = times;
            // res->val = par1->val * par2->val;
            res->val = par1->val;
            res->left = par1;
            res->right = par2;
            // res = par1 * par2;
        }
        else if (temp == divide)
        {
            // printf("td ");
            res = malloc(sizeof(NodeDesc));
            res->kind = divide;
            // res->val = par1->val / par2->val;
            res->val = par1->val;
            res->left = par1;
            res->right = par2;
            // res = par1 / par2;
        }
        else if (temp == mod)
        {
            // printf("tm ");
            res = malloc(sizeof(NodeDesc));
            res->kind = mod;
            // res->val = par1->val % par2->val;
            res->val = par1->val;
            res->left = par1;
            res->right = par2;
            // res = par1 % par2;
        }
        par1 = res;
    }
    return res;
}
static Node Expr()
{
    if ((sym == minus) || (sym == plus))
        sym = SGet();

    register Node res;
    register Node par1 = Term();

    res = par1;
    while ((sym == plus) || (sym == minus))
    {

        int temp = sym;
        sym = SGet();
        register Node par2 = Term();

        res = malloc(sizeof(NodeDesc));
        res->kind = temp;
        res->val = par1->val;
        res->left = par1;
        res->right = par2;

        par1 = res;
    }
    return res;
}
int main(int argc, char *argv[])
{
    fp = fopen("program.asm", "w");

    fprintf(fp, ".text # text section \n.globl main # call main by SPIM \nmain:\n");

    register Node result;
    if (argc == 2)
    {
        SInit(argv[1]);
        sym = SGet();
        Node result = Expr();
        assert(sym == eof);

        GenMIPs(result, 0);
    }
    else
    {
        printf("usage: expreval <filename>\n");
    }

    fprintf(fp, "li   $v0, 1\nsyscall\n");
    fprintf(fp, "end:\nori   $v0, $0, 10  # system call 10 for exit\nsyscall            # we are out of here.\n");

    return 0;
}

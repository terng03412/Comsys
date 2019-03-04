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
    illegal,
    var
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
    case 'x':
        sym = var;
        ch = getc(f);
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
    assert((sym == number) || (sym == lparen) || (sym = var));
    if ((sym == number) || (sym == var))
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

        // printf("tt ");
        res = malloc(sizeof(NodeDesc));
        res->kind = temp;
        // res->val = par1->val * par2->val;
        res->val = par1->val;
        res->left = par1;
        res->right = par2;
        // res = par1 * par2;
        par1 = res;
    }
    return res;
}

static Node Expr()
{
    register Node result, root;

    root = Term();
    while (sym == plus || sym == minus)
    {
        result = malloc(sizeof(NodeDesc));
        result->kind = sym;
        sym = SGet();
        result->left = root;
        result->right = Term();
        root = result;
    }
    return root;
}

static void prefix(Node root)
{
    if (root != NULL)
    {
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
        case var:
            printf("x");
            break;
        case number:
            printf("%d", root->val);
            break;
        }
        printf(" ");

        prefix(root->left);
        prefix(root->right);
    }
}

static void postfix(Node root)
{
    if (root != NULL)
    {
        postfix(root->left);
        postfix(root->right);

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
        case var:
            printf("x");
            break;
        case number:
            printf("%d", root->val);
            break;
        }
        printf(" ");
    }
}

static void infix(Node root)
{
    if (root != NULL)
    {
        if (root->left != NULL && root->right != NULL)
        {
            printf("( ");
        }
        infix(root->left);

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
        case var:
            printf("x");
            break;
        case number:
            printf("%d", root->val);
            break;
        }
        printf(" ");

        infix(root->right);
        if (root->left != NULL && root->right != NULL)
        {
            printf(") ");
        }
    }
}

static Node diff(Node root)
{

    register Node result, l, r, d, ll, lr;

    if ((root->kind == number) || (root->kind == var))
    {
        result = malloc(sizeof(NodeDesc));
        result->kind = number;

        if (root->kind == number)
            result->val = 0;
        else
            result->val = 1;

        result->left = NULL;
        result->right = NULL;
        return result;
    }
    else if ((root->kind == plus) || (root->kind == minus))
    {
        result = malloc(sizeof(NodeDesc));
        result->kind = root->kind;
        result->left = diff(root->left);
        result->right = diff(root->right);
        return result;
    }
    else if (root->kind == times)
    {
        result = malloc(sizeof(NodeDesc));
        result->kind = plus;

        l = malloc(sizeof(NodeDesc));
        l->kind = times;
        l->left = diff(root->left);
        l->right = root->right;

        r = malloc(sizeof(NodeDesc));
        r->kind = times;
        r->left = root->left;
        r->right = diff(root->right);

        result->left = l;
        result->right = r;
        return result;
    }
    else if (root->kind == divide)
    {
        result = malloc(sizeof(NodeDesc));
        result->kind = divide;

        l = malloc(sizeof(NodeDesc));
        l->kind = minus;
        ll = malloc(sizeof(NodeDesc));
        ll->kind = times;
        ll->left = root->right;
        ll->right = diff(root->left);

        lr = malloc(sizeof(NodeDesc));
        lr->kind = times;
        lr->left = root->left;
        lr->right = diff(root->right);

        l->left = ll;
        l->right = lr;

        d = malloc(sizeof(NodeDesc));
        d->kind = times;
        d->left = root->right;
        d->right = root->right;

        result->left = l;
        result->right = d;

        return result;
    }
    else
    {
        printf("Unsupported operator %d\n", root->kind);
    }
    return result;
}

static int treeEq(Node LeftRoot, Node RightRoot)
{
    if ((LeftRoot == NULL) && (RightRoot == NULL))
        return 1;
    else if ((LeftRoot == NULL) || (RightRoot == NULL))
        return 0;
    else if ((LeftRoot->kind == number) && (RightRoot->kind == number))
        if (LeftRoot->val == RightRoot->val)
            return 1;
        else
            return 0;
    else if ((LeftRoot->kind == var) && (RightRoot->kind == var))
        return 1;

    if (!treeEq(LeftRoot->left, RightRoot->left))
        return 0;
    if (!treeEq(LeftRoot->right, RightRoot->right))
        return 0;
    if (LeftRoot->kind == RightRoot->kind)
        return 1;
    return 0;
}

static Node simplify(Node root, int *ptrSim)
{
    register Node result;

    if (root == NULL)
        return root;

    if ((root->kind == number) || (root->kind == var))
        return root;

    if (root->kind == times)
    {
        if (((root->left)->kind == number) && ((root->left)->val == 0))
        {
            *ptrSim = 1;
            return root->left;
        }

        if (((root->right)->kind == number) && ((root->right)->val == 0))
        {
            *ptrSim = 1;
            return root->right;
        }

        if (((root->left)->kind == number) && ((root->left)->val) == 1)
        {
            *ptrSim = 1;
            return root->right;
        }

        if (((root->right)->kind == number) && ((root->right)->val == 1))
        {
            *ptrSim = 1;
            return root->left;
        }
    }
    else if (root->kind == plus)
    {

        if (((root->left)->kind == number) && ((root->left)->val == 0))
        {
            *ptrSim = 1;
            return root->right;
        }

        if (((root->right)->kind == number) && ((root->right)->val == 0))
        {
            *ptrSim = 1;
            return root->left;
        }

        if (treeEq(root->left, root->right) == 1)
        {
            *ptrSim = 1;
            result = malloc(sizeof(NodeDesc));
            result->kind = number;
            result->val = 2;
            result->left = NULL;
            result->right = NULL;
            root->left = result;
            root->kind = times;
            return root;
        }
    }
    else if (root->kind == minus)
    {
        if (treeEq(root->left, root->right) != 0)
        {
            *ptrSim = 1;
            result = malloc(sizeof(NodeDesc));
            result->kind = number;
            result->val = 0;
            result->left = NULL;
            result->right = NULL;
            root = result;
            return root;
        }
    }
    root->left = simplify(root->left, ptrSim);
    root->right = simplify(root->right, ptrSim);
    return root;
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

int main(int argc, char *argv[])
{
    int simp;
    register Node result, Dif, simR;

    if (argc == 2)
    {
        SInit(argv[1]);
        sym = SGet();
        result = Expr();
        assert(sym == eof);
        // printf("result = %d\n", result->val);
        // Print(result, 0);

        printf("prefix: \t");
        prefix(result);
        printf("\n");

        printf("infix: \t\t");
        infix(result);
        printf("\n");

        printf("postfix: \t");
        postfix(result);
        printf("\n");

        Dif = diff(result);
        printf("dif infix: \t");
        infix(Dif);
        printf("\n");

        simR = Dif;
        simp = 1;

        while (simp)
        {
            simp = 0;
            simR = simplify(simR, &simp);
        }
        printf("simp infix: \t");
        infix(simR);
        printf("\n");
        printf("simp tree: \t");
        Print(simR, 0);
        printf("\n");
    }
    else
    {
        printf("usage: expreval <filename>\n");
    }
    return 0;
}

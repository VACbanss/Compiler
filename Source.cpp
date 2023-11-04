#include <string>
#include <iostream>

/*Lexer*/
enum { // ����������� ������� �����
	DO_SYM, ELSE_SYM, IF_SYM, WHILE_SYM, LBRA, RBRA, LPAR, RPAR,
	PLUS, MINUS, LESS, SEMI, EQUAL, INT, ID, EOI
};

const char* words[] = {"do", "else", "if", "while", NULL}; // �������� �����
int ch = ' ';
int sym;
int int_val;
char id_name[100];

void syntax_error() { fprintf(stderr, "syntax error\n"); exit(1); } // ������ ������
void next_ch() { ch = getchar();} // ��������� ������ ������
void next_sym() // ������ ������ �� ������������ ���������
{
again: switch (ch)
{
case ' ': case '\n': next_ch(); goto again;
case EOF: sym = EOI; break;
case '{': next_ch(); sym = LBRA; break;
case '}': next_ch(); sym = RBRA; break;
case '(': next_ch(); sym = LPAR; break;
case ')': next_ch(); sym = RPAR; break;
case '+': next_ch(); sym = PLUS; break;
case '-': next_ch(); sym = MINUS; break;
case '<': next_ch(); sym = LESS; break;
case ';': next_ch(); sym = SEMI; break;
case '=': next_ch(); sym = EQUAL; break;
default:
    if (ch >= '0' && ch <= '9')
    {
        int_val = 0;
        while (ch >= '0' && ch <= '9')
        {
            int_val = int_val * 10 + (ch - '0'); next_ch();
        }
        sym = INT;
    }
    else if (ch >= 'a' && ch <= 'z')
    {
        int i = 0;
        while ((ch >= 'a' && ch <= 'z') || ch == '_')
        {
            id_name[i++] = ch; next_ch();
        }
        id_name[i] = '\0';
        sym = 0;
        while (words[sym] != NULL && strcmp(words[sym], id_name) != 0)
            sym++;
        if (words[sym] == NULL)
            if (id_name[1] == '\0') sym = ID; else syntax_error();
    }
    else
        syntax_error();
}
}

/*Parser*/

enum {
    VAR, CST, ADD, SUB, LT, SET,
    IF1, IF2, WHILE, DO, EMPTY, SEQ, EXPR, PROG
};

struct node { int kind; struct node* o1, * o2, * o3; int val; };
typedef struct node node;

node* new_node(int k)
{
    node* x = (node*)malloc(sizeof(node)); x->kind = k; return x;
}

node* paren_expr();

node* term()
{
    node* x;
    if (sym == ID) { x = new_node(VAR); x->val = id_name[0] - 'a'; next_sym(); }
    else if (sym == INT) { x = new_node(CST); x->val = int_val; next_sym(); }
    else x = paren_expr();
    return x;
}

node* sum()
{
    node* t, * x = term();
    while (sym == PLUS || sym == MINUS)
    {
        t = x; x = new_node(sym == PLUS ? ADD : SUB); next_sym(); x->o1 = t; x->o2 = term();
    }
    return x;
}

node* test()
{
    node* t, * x = sum();
    if (sym == LESS)
    {
        t = x; x = new_node(LT); next_sym(); x->o1 = t; x->o2 = sum();
    }
    return x;
}

node* expr() 
{
    node* t, * x;
    if (sym != ID) return test();
    x = test();
    if (x->kind == VAR && sym == EQUAL)
    {
        t = x; x = new_node(SET); next_sym(); x->o1 = t; x->o2 = expr();
    }
    return x;
}

node* paren_expr() 
{
    node* x;
    if (sym == LPAR) next_sym(); else syntax_error();
    x = expr();
    if (sym == RPAR) next_sym(); else syntax_error();
    return x;
}

node* statement()
{
    node* t, * x;
    if (sym == IF_SYM)
    {
        x = new_node(IF1);
        next_sym();
        x->o1 = paren_expr();
        x->o2 = statement();
        if (sym == ELSE_SYM)
        {
            x->kind = IF2;
            next_sym();
            x->o3 = statement();
        }
    }
    else if (sym == WHILE_SYM)
    {
        x = new_node(WHILE);
        next_sym();
        x->o1 = paren_expr();
        x->o2 = statement();
    }
    else if (sym == DO_SYM)
    {
        x = new_node(DO);
        next_sym();
        x->o1 = statement();
        if (sym == WHILE_SYM) next_sym(); else syntax_error();
        x->o2 = paren_expr();
        if (sym == SEMI) next_sym(); else syntax_error();
    }
    else if (sym == SEMI)
    {
        x = new_node(EMPTY); next_sym();
    }
    else if (sym == LBRA)
    {
        x = new_node(EMPTY);
        next_sym();
        while (sym != RBRA)
        {
            t = x; x = new_node(SEQ); x->o1 = t; x->o2 = statement();
        }
        next_sym();
    }
    else
    {
        x = new_node(EXPR);
        x->o1 = expr();
        if (sym == SEMI) next_sym(); else syntax_error();
    }
    return x;
}

node* program()
{
    node* x = new_node(PROG);
    next_sym(); x->o1 = statement(); if (sym != EOI) syntax_error();
    return x;
}

/*Code generator*/

enum { IFETCH, ISTORE, IPUSH, IPOP, IADD, ISUB, ILT, JZ, JNZ, JMP, HALT };

typedef char code;
code object[1000], * here = object;

void g(code c) { *here++ = c; }
code* hole() { return here++; }
void fix(code* src, code* dst) { *src = dst - src; }

void c(node* x)
{
    code* p1, * p2;
    switch (x->kind)
    {
    case VAR: g(IFETCH); g(x->val); break;
    case CST: g(IPUSH); g(x->val); break;
    }
}

int main()
{
	
    return 0;
}
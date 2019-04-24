#include<stdbool.h>
#include<string.h>
extern int yylineno;
int yyerror(const char *msg);
struct ast
{
    char *nodetype;
    char *nodename;
    int opnum;
    double doubleval;
    int intval;
    int lineno;
    struct ast *child;
    struct ast *sibling;
};

struct numval
{
    int nodetype;
    double number;
};

struct symbol
{
    char *name;
    double doubleval;
    int intval;
    struct ast *func;
    struct ast *args;
};


struct ast *newast(char *nodetype, int opnum, int lineno,char *nodename);
void add_child(struct ast* node,struct ast *child);
void add_sibling(struct ast* node,struct ast *sibling);
void print_ast(struct ast* node,int tabs);

double eval(struct ast*);

void treefree(struct ast*);

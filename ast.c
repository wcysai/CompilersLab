#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "defi.h"
#define TABWIDTH 4

struct ast *

newast(char *nodetype, int opnum, int lineno, char *nodename)
{
    struct ast *a=malloc(sizeof(struct ast));
    if(!a)
    {
        yyerror("no free space");
        exit(-1);
    }
    a->nodetype=nodetype;
    a->lineno=lineno; a->opnum=opnum;
    a->nodename=nodename;
    a->child=NULL; a->sibling=NULL;
    a->intval=0; a->doubleval=0;
    return a;
}

void add_child(struct ast *node,struct ast *child)
{
    node->child=child;
}

void add_sibling(struct ast *node, struct ast *sibling)
{
    if(node->sibling==NULL) node->sibling=sibling;
    else add_sibling(node->sibling,sibling);
}

void print_ast(struct ast *node,int tabs)
{
    if(node==NULL) return;
    for(int i=0;i<tabs;i++) printf("\t");
    if(!strcmp(node->nodetype,"INT"))
    {
        printf("INT: %d\n",node->intval);
        return;
    }
}


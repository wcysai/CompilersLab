#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "defi.h"
struct ast *
newast(char *nodetype, int lineno, char *nodename)
{
    struct ast *a=malloc(sizeof(struct ast));
    if(!a)
    {
        yyerror("no free space");
        exit(-1);
    }
    a->nodetype=nodetype;
    a->lineno=lineno;
    a->nodename=nodename;
    a->child=NULL; a->sibling=NULL;
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



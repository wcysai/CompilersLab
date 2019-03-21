#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
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
    if(node==NULL) return;
    node->child=child;
}

void add_sibling(struct ast *node, struct ast *sibling)
{
    if(node==NULL) return;
    if(node->sibling==NULL) node->sibling=sibling;
    else assert(0);
}

void print_ast(struct ast *node,int tabs)
{
    if(node==NULL) return;
    for(int i=0;i<tabs;i++) printf("  ");
    if(!strcmp(node->nodetype,"INT"))
    {
        printf("INT: %d\n",node->intval);
    }
    else if(!strcmp(node->nodetype,"FLOAT"))
    {
        printf("FLOAT: %.10f\n",node->doubleval);
    }
    else if(!strcmp(node->nodetype,"ID"))
    {
       printf("ID: %s\n",node->nodename);
    }
    else if(!strcmp(node->nodetype,"TYPE"))
    {
        printf("TYPE: %s\n",node->nodename);
    }
    else 
    {
        if(node->child==NULL)
        {
            printf("%s\n",node->nodetype);
        }
        else
        {
            printf("%s (%d)\n",node->nodetype,node->lineno);
            print_ast(node->child,tabs+1);
        }
    }
    if(node->sibling) print_ast(node->sibling,tabs);
}


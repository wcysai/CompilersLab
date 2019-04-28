#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "defi.h"
#define TABWIDTH 4

ast newast(char *nodetype, int opnum, int lineno, char *nodename)
{
    ast a=malloc(sizeof(struct ast_));
    if(!a)
    {
        yyerror("no free space");
        exit(-1);
    }
    a->nodetype=nodetype;
    a->lineno=lineno; a->opnum=opnum;
    a->nodename=nodename;
    a->child=NULL; a->sibling=NULL; a->parent=NULL;
    a->val.intval=0;
    return a;
}

void add_child(ast node,ast child)
{
    if(node==NULL) return;
    node->child=child; child->parent=node;
}

void add_sibling(ast node,ast sibling)
{
    if(node==NULL) return;
    if(node->sibling==NULL) 
    {
        node->sibling=sibling;
        sibling->parent=node->parent;
    }
}

void print_ast(ast node,int tabs)
{
    if(node==NULL) return;  
    if(strcmp(node->nodetype,"Empty")) 
    {
        for(int i=0;i<tabs;i++) printf("  ");
        if(!strcmp(node->nodetype,"INT"))
        {
            printf("INT: %d\n",node->val.intval);
        }
        else if(!strcmp(node->nodetype,"FLOAT"))
        {
            printf("FLOAT: %.10f\n",node->val.doubleval);
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
    }
    if(node->sibling) print_ast(node->sibling,tabs);
}

char* extract_name(ast node)
{
    if(!strcmp(node->nodetype,"Empty")) return NULL;
    if(!strcmp(node->nodetype,"ID")) return node->nodename;
    else if(!strcmp(node->nodetype,"Tag")) return p1(node)->nodename;
    else if(!strcmp(node->nodetype,"OptTag"))
    {
        switch(node->opnum)
        {
            case 1: 
            {
                return p1(node)->nodename;
                break;
            }
            case 2: return NULL;
        }
    }
    return node->nodename;
}



#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "defi.h"
struct ast *
newast(int nodetype, struct ast *l,struct ast *r)
{
    struct ast *a=malloc(sizeof(struct ast));
    if(!a)
    {
        yyerror("no free space");
        exit(-1);
    }
    a->nodetype=nodetype;
    a->l=l; a->r=r;
    return a;
}



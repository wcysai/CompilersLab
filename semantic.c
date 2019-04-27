#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "defi.h"


Symbol construct_symbol(ast node)
{
    Symbol sy=malloc(sizeof(struct Symbol_));
    if(!strcmp(node->nodetype,"Def"))
    {
        sy->lineno=node->lineno;
    }
    return sy;
}

void semantic_analysis(ast node)
{
    if(node==NULL) return;
    if(!strcmp(node->nodetype,"Def"))
    {
        
    }
}

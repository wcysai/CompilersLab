#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "defi.h"

bool SameType(Type p,Type q)
{
    if(p==NULL&&q==NULL) return true;
    if(p==NULL||q==NULL) return false;
    if(p->kind!=q->kind) return false;
    switch(p->kind)
    {
        case BASIC: return p->u.basic==q->u.basic;
        case ARRAY: return p->u.array.elem==q->u.array.elem;
        case STRUCTURE: return SameField(p->u.structure,q->u.structure);
    }
}

bool SameField(FieldList p,FieldList q)
{
    if(p==NULL&&q==NULL) return true;
    if(p==NULL||q==NULL) return false;
    return SameType(p->type,q->type)&&SameField(p->tail,q->tail);
}


Type construct_basic(ast node)
{
    Type ty=malloc(sizeof(struct Type_));
    ty->kind=BASIC;
    if(!strcmp(node->nodename,"int")) ty->u.basic=0;
    else ty->u.basic=1;
}

Type construct_struct(ast node)
{
    Type ty=malloc(sizeof(struct Type_));
    ty->kind=STRUCTURE;
    switch(node->opnum)
    {
        case 1: 
        {
            ty->u.structure->name=extract_name(node->child->sibling);
        }
    }
}

Type construct_type(ast node)
{
    Type ty=malloc(sizeof(struct Type_));
    switch(node->opnum)
    {
        case 1: return NULL;
    }
}


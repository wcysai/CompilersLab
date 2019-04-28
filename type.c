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
            char *name=extract_name(p2(node));
            Symbol sym=lookup(name,1);
            if(sym==NULL||sym->SymbolType!=Variable||sym->u.type->kind!=STRUCTURE) {printf("Error type 17 at line %d: Undefined struct %s\n",node->lineno,name);}
            ty=sym->u.type;
            break;
        }
        case 2:
        {
            DefList df=construct_DefList(p4(node));
            FieldList fl=malloc(sizeof(struct FieldList_));
            while(df!=NULL)
            {
                FieldList nfl=malloc(sizeof(struct FieldList_));
                nfl->name=df->name;
                nfl->type=df->type;
                nfl->tail=fl;
                df=df->tail;
                fl=nfl;
            }
            ty->u.structure=fl;
        }
    }
    return ty;
}

Type construct_type(ast node)
{
    switch(node->opnum)
    {
        case 1: {return construct_basic(p1(node)); break;}
        case 2: {return construct_struct(p1(node)); break;}
    }
}


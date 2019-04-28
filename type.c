#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "defi.h"

bool SameSymbol(Symbol p,Symbol q)
{
    if(p==NULL&&q==NULL) return true;
    if(p==NULL||q==NULL) return false;
    if(p->SymbolType!=q->SymbolType) return false;
    switch(p->SymbolType)
    {
        case Function: return SameFunction(p->u.func,q->u.func);
        case Variable: return SameType(p->u.type,q->u.type);
    }
}

bool SameFunction(FunDec p,FunDec q)
{
    if(p==NULL&&q==NULL) return true;
    if(p==NULL||q==NULL) return false;
    if(!SameType(p->type,q->type)) return false;
    return SameVarList(p->args,q->args);
}

bool SameVarList(VarList p,VarList q)
{
    if(p==NULL&&q==NULL) return true;
    if(p==NULL||q==NULL) return false;
    if(!SameType(p->type,q->type)) return false;
    return SameVarList(p->tail,q->tail);
}

bool IsInt(Type p)
{
    return (p->kind==BASIC&&p->u.basic==0);
}

bool IsFloat(Type p)
{
    return (p->kind==BASIC&&p->u.basic==1);
}

bool IsArray(Type p)
{
    return (p->kind==ARRAY);
}

bool IsStruct(Type p)
{
    return (p->kind==STRUCTURE);
}

bool IsVariable(Symbol p)
{
    return p->SymbolType==Variable;
}

bool IsFunction(Symbol p)
{
    return p->SymbolType==Function;
}

Type newint()
{
    Type tp=malloc(sizeof(struct Type_));
    tp->kind=BASIC;
    tp->u.basic=0;
}

Type newfloat()
{
    Type tp=malloc(sizeof(struct Type_));
    tp->kind=BASIC;
    tp->u.basic=1;
}

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
    return ty;
}

Type search_struct(Type tp,char *name)
{
    FieldList fl=tp->u.structure;
    while(fl!=NULL)
    {
        if(!strcmp(fl->name,name)) return fl->type;
        fl=fl->tail;
    }
    return NULL;
}

Type construct_struct(ast node)
{
    Type ty=malloc(sizeof(struct Type_));
    ty->kind=STRUCTURE;
    switch(node->opnum)
    {
        case 2: 
        {
            char *name=extract_name(p2(node));
            Symbol sym=Variable_Lookup(name);
            if(sym==NULL||sym->SymbolType!=Variable||sym->u.type->kind!=STRUCTURE) 
            {
                char msg[100];
                sprintf(msg,"Undefined struct \"%s\"",name);
                semantic_error(17,node->lineno,msg);
                ty->u.structure=NULL;
                return ty;
            }
            ty=sym->u.type;
            break;
        }
        case 1:
        {
            DefList df=construct_DefList(p4(node));
            FieldList fl=NULL;
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


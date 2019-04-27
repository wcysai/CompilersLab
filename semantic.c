#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "defi.h"

Dec construct_Dec(ast node)
{
    Dec dc=malloc(sizeof(struct Dec_));
    switch(node->opnum)
    {
        case 1: 
        {
            dc->name=extract_name(p1(node));
            dc->array=NULL;
            break;
        }
        case 2:
        {
            dc=construct_Dec(p1(node));
            ArrayDec ad=malloc(sizeof(struct ArrayDec_));
            ad->size=p3(node)->val.intval; 
            ad->tail=dc->array;
            dc->array=ad;
            break;
        }
    }
    return dc;
}

DecList construct_DecList(ast node)
{
    DecList dl=malloc(sizeof(struct DecList_));
    dl->dec=construct_Dec(p1(p1(node)));
    switch(node->opnum)
    {
        case 1: {dl->tail=NULL; break;}
        case 2: {dl->tail=construct_DecList(p3(node)); break;}
    }
    return dl;
}

Type arrtype(Type tp,ArrayDec arr)
{
    while(arr!=NULL)
    {
        Type ntp=malloc(sizeof(struct Type_));
        ntp->kind=ARRAY;
        ntp->u.array.elem=tp; ntp->u.array.size=arr->size;
        arr=arr->tail;
        tp=ntp;
    }
    return tp;
}

DefList construct_Def(ast node)
{
    if(!strcmp(node->nodename,"Empty")) return NULL;
    Type tp=construct_type(p1(node));
    assert(tp!=NULL);
    DecList dl=construct_DecList(p2(node));
    DefList df=malloc(sizeof(struct DefList_));
    while(dl!=NULL)
    {
        DefList ndf=malloc(sizeof(struct DefList_));
        ndf->name=dl->dec->name;
        ndf->type=arrtype(tp,dl->dec->array);
        ndf->tail=df;
        dl=dl->tail; df=ndf;
    }
    return df;
}

DefList construct_DefList(ast node)
{ 
    if(!strcmp(node->nodename,"Empty")) return NULL;
    switch(node->opnum)
    {
        case 2: {return NULL; break;}
        case 1:
        {
            DefList df=construct_Def(p1(node));
            DefList app=construct_DefList(p2(node));
            df->tail=app;
            return df;
            break;
        }
    }
}


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
    semantic_analysis(p1(node));
    semantic_analysis(node->sibling);
    if(!strcmp(node->nodetype,"DefList"))
    {
        DefList dl=construct_DefList(node); 
        printf("%s\n",dl->name);
    }
}

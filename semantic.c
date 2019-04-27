#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "defi.h"

Dec construct_Dec(ast node)
{
    Dec dc=malloc(sizeof(struct Dec_));
    node=p1(node);
    switch(node->opnum)
    {
        case 1: 
        {
            dc->name=extract_name(p1(node));
            dc->array=NULL;
        }
        case 2:
        {
            dc=construct_Dec(p1(node));
            ArrayDec ad=malloc(sizeof(struct ArrayDec_));
            ad->size=p3(node)->val.intval; 
            ad->tail=dc->array;
            dc->array=ad;
        }
    }
    return dc;
}

DecList construct_DecList(ast node)
{
    DecList dl=malloc(sizeof(struct DecList_));
    dl->dec=construct_Dec(node->child);
    switch(node->opnum)
    {
        case 1: dl->tail=NULL;
        case 2: dl->tail=construct_DecList(p3(node));
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
    Type tp=construct_type(p1(node));
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
    switch(node->opnum)
    {
        case 1:return NULL;
        case 2:
        {
            DefList df=construct_Def(p1(node));
            DefList app=construct_DefList(p2(node));
            df->tail=app;
            return df;
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
    if(!strcmp(node->nodetype,"Def"))
    {
        
    }
}

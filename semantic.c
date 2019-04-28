#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "defi.h"

Dec construct_VarDec(ast node)
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
            dc=construct_VarDec(p1(node));
            ArrayDec ad=malloc(sizeof(struct ArrayDec_));
            ad->size=p3(node)->val.intval; 
            ad->tail=dc->array;
            dc->array=ad;
            break;
        }
    }
    return dc;
}

Dec construct_Dec(ast node)
{
    return construct_VarDec(p1(node));
}

DecList construct_DecList(ast node)
{
    DecList dl=malloc(sizeof(struct DecList_));
    dl->dec=construct_Dec(p1(node));
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

VarList construct_VarList(ast node)
{
    VarList vl=malloc(sizeof(struct VarList_));
    Type tp=construct_type(p1(p1(node)));
    Dec dec=construct_VarDec(p2(p1(node)));
    tp=arrtype(tp,dec->array);
    vl->name=dec->name;
    vl->type=tp;
    switch(node->opnum)
    {
        case 1:
        {
            vl->tail=NULL;
            break;
        }
        case 2:
        {
            vl->tail=construct_VarList(p3(node));
            break;
        }
    }
    return vl;
}

FunDec construct_FunDec(ast node)
{
    Type tp=construct_type(p1(node));
    FunDec fd=malloc(sizeof(struct FunDec_));
    fd->type=tp;
    fd->name=extract_name(p1(p1(node)));
    switch(p2(node)->opnum)
    {
        case 2:
        {
            fd->args=NULL;
            break;
        }
        case 1:
        {
            fd->args=construct_VarList(p3(p1(node)));
            break;
        }
    }
    return fd;
}


Symbol construct_variable_symbol(char* name,Type tp)
{
    Symbol sy=malloc(sizeof(struct Symbol_));
    sy->name=name;
    sy->SymbolType=1;
    sy->u.type=tp;
    return sy;
}

void Definition_analysis(ast node)
{
    if(!strcmp(node->nodetype,"DefList"))
    {
        DefList dl=construct_DefList(node);
        while(dl!=NULL)
        {
            Symbol sym=construct_symbol(dl->name,dl->type);
            if()
        }
    }
}

void semantic_analysis(ast node)
{
    if(node==NULL) return;
    semantic_analysis(p1(node));
    semantic_analysis(node->sibling);
    if(!strcmp(node->nodetype,"DefList")||!strcmp(node->nodetype,"ExtDefList")) Definition_analysis(node);
}

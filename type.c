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

Dec construct_Dec(ast node)
{
    Dec dc=malloc(sizeof(struct Dec_));
    node=node->child;
    switch(node->opnum)
    {
        case 1: 
        {
            dc->name=extract_name(node->child);
            dc->array=NULL;
        }
        case 2:
        {
            dc=construct_Dec(node->child);
            ArrayDec ad=malloc(sizeof(struct ArrayDec_));
            ad->size=node->child->sibling->sibling->val.intval; 
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
        case 2: dl->tail=construct_DecList(node->child->sibling->sibling);
    }
    return dl;
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

FieldList construct_type_list(ast node)
{
    FieldList fi=malloc(sizeof(struct FieldList_));
    switch(node->opnum)
    {
        case 1:
        {
            fi->type=construct_type(node->child);
            fi->tail=construct_type_list(node->child->sibling);
        }
        case 2:return NULL;
    }

}

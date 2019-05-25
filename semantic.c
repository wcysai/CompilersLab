#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "defi.h"

void semantic_error(int type,int lineno,const char* msg)
{
    printf("Error type %d at Line %d: %s\n",type,lineno,msg);
}

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
    DecList dl=construct_DecList(p2(node)); 
    DefList df=NULL;
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
            DefList pt=df;
            while(pt->tail!=NULL) pt=pt->tail; 
            pt->tail=app;
            return df;
            break;
        }
    }
}

VarList construct_Args(ast node)
{
    VarList vl=malloc(sizeof(struct VarList_));
    Type tp=construct_expression_type(p1(node));
    vl->type=tp;
    switch(node->opnum)
    {
        case 2:
        {
            vl->tail=NULL;
            break;
        }
        case 1:
        {
            vl->tail=construct_Args(p3(node));
            break;
        }
    }
    return vl;
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
        case 2:
        {
            vl->tail=NULL;
            break;
        }
        case 1:
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
    fd->name=extract_name(p1(p2(node)));
    switch(p2(node)->opnum)
    {
        case 2:
        {
            fd->args=NULL;
            break;
        }
        case 1:
        {
            fd->args=construct_VarList(p3(p2(node)));
            break;
        }
    }
    return fd;
}

Type construct_expression_type(ast node)
{
    if(!strcmp(node->nodetype,"Empty")) return NULL;
    switch(node->opnum)
    {
        case 1:
        {
            char* name=extract_name(p1(node));
            Symbol sym=Variable_Lookup(name);
            if(sym==NULL)
            {
                char msg[100];
                sprintf(msg,"Undefined variable \"%s\"",name);
                semantic_error(1,node->lineno,msg);
                return NULL;
            }
            return sym->u.type;
        }
        case 2: {return newint();}
        case 3: {return newfloat();}
        case 4: 
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16: {return construct_expression_type(p1(node));}
        case 17: 
        case 18:
        case 19: {return construct_expression_type(p2(node));}
        case 20:
        case 21:
        {
            char* name=extract_name(p1(node));
            Symbol sym=Function_Lookup(name);
            if(sym==NULL)
            {
                Symbol syms=Variable_Lookup(name);
                if(syms!=NULL)
                {
                    char msg[100];
                    sprintf(msg,"\"%s\" is not a function",name);
                    semantic_error(11,node->lineno,msg);
                    return NULL;
                }
                char msg[100];
                sprintf(msg,"Undefined function \"%s\"",name);
                semantic_error(2,node->lineno,msg);
                return NULL;
            }
            return sym->u.func->type;
        }
        case 22:
        {
            Type tp=construct_expression_type(p1(node));
            if(!IsArray(tp)) return NULL;
            return tp->u.array.elem;
        }
        case 23:
        {
            char* name=extract_name(p3(node));
            Symbol sym=Variable_Lookup(name);
            if(sym==NULL) return NULL; else return sym->u.type;
        }
    }
}

void expression_semantic_analysis(ast node)
{
    if(!strcmp(node->nodetype,"Empty")) return;
    switch(node->opnum) 
    {
        case 1: 
        case 2: 
        case 3: return;
        case 4: 
        {
            Type tp1=construct_expression_type(p1(node));
            Type tp2=construct_expression_type(p3(node));
            if(tp1==NULL||tp2==NULL) return;
            if(!SameType(tp1,tp2))
            {
                char msg[100];
                sprintf(msg,"Type mismatched for assignment");
                semantic_error(5,node->lineno,msg);
                return;
            }
            if(p1(node)->opnum==2||p1(node)->opnum==3||p1(node)->opnum==20||p1(node)->opnum==21)
            {
                char msg[100];
                sprintf(msg,"The left-hand side of an assignment must be a variable.");
                semantic_error(6,node->lineno,msg);
                return;
            }
            break;
        }
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:  
        {
            Type tp1=construct_expression_type(p1(node));
            Type tp2=construct_expression_type(p3(node));
            if(tp1==NULL||tp2==NULL) return;
            if(!(IsInt(tp1)&&IsInt(tp2))&&!(IsFloat(tp1)&&IsFloat(tp2)))
            {
                char msg[100];
                sprintf(msg,"Type mismatched for operands.");
                semantic_error(7,node->lineno,msg);
                return;
            }
            break;
        }
        case 17: return; 
        case 18:
        case 19: 
        {
            Type tp=construct_expression_type(p2(node));
            if(tp==NULL) return;
            if(!IsInt(tp)&&!IsFloat(tp))
            {
                char msg[100];
                sprintf(msg,"Type mismatched for operands.");
                semantic_error(7,node->lineno,msg);
                return;
            }
            break;
        }
        case 20:
        {
            Type tp=construct_expression_type(node);
            if(tp==NULL) return;
            char* name=extract_name(p1(node));
            Symbol sym=Function_Lookup(name);
            VarList args=construct_Args(p3(node));
            if(!SameVarList(args,sym->u.func->args))
            {
                char msg[100];
                sprintf(msg,"Function is not applicable for the given arguments");
                semantic_error(9,node->lineno,msg);
                return;
            }
            break;
        }
        case 21:
        {
            Type tp=construct_expression_type(node);
            if(tp==NULL) return;
            char* name=extract_name(p1(node));
            Symbol sym=Function_Lookup(name);
            VarList args=sym->u.func->args;
            if(args!=NULL)
            {
                char msg[100];
                sprintf(msg,"Function is not applicable for the given arguments.");
                semantic_error(9,node->lineno,msg);
                return;
            }
            break;
        }
        case 22:
        {
            Type tp1=construct_expression_type(p1(node));
            Type tp2=construct_expression_type(p3(node));
            if(!IsArray(tp1))
            {
                char *name=extract_name(p1(p1(node)));
                char msg[100];
                sprintf(msg,"\"%s\" is not an array.",name);
                semantic_error(10,node->lineno,msg);
                return;
            }
            if(!IsInt(tp2))
            {
                char msg[100];
                sprintf(msg,"Array indices can only be of integer type");
                semantic_error(12,node->lineno,msg);
                return;
            }
            break;
        }
        case 23:
        {
            Type tp=construct_expression_type(p1(node));
            if(!IsStruct(tp))
            {
                char msg[100];
                sprintf(msg,"Illegal use of \".\"");
                semantic_error(13,node->lineno,msg);
                return;
            }
            char* name=extract_name(p3(node));
            Type tt=search_struct(tp,name);
            if(tt==NULL)
            {
                char msg[100];
                sprintf(msg,"Nonexistent field \"%s\"",name);
                semantic_error(14,node->lineno,msg);
                return;
            }
            break;
        }
    }
}


Symbol construct_variable_symbol(char* name,Type tp)
{
    Symbol sy=malloc(sizeof(struct Symbol_));
    sy->name=name;
    sy->SymbolType=1;
    sy->u.type=tp;
    return sy;
}

Symbol construct_function_symbol(char* name,FunDec fd)
{
    Symbol sy=malloc(sizeof(struct Symbol_));
    sy->name=name;
    sy->SymbolType=0;
    sy->u.func=fd;
    return sy;
}

void ExtDef(ast node)
{
    switch(node->opnum)
    {
        case 1:{Define_Variable(node); break;}
        case 2:{Define_Struct(node); break;}
        case 3:{Define_Function(node); break;}
    }
}

void Define_Struct(ast node)
{
    node=p1(node);
    Type tp=construct_type(node);
    char* name=extract_name(p2(p1(node)));
    Symbol sym=construct_variable_symbol(name,tp); 
    if(Variable_Lookup(sym->name)!=NULL)
    {
        char msg[100];
        sprintf(msg,"Duplicated name \"%s\"",name);
        semantic_error(16,node->lineno,msg);
        return;
    }
    if(!trieinsert(sym))
    {
        char msg[100];
        sprintf(msg,"Redefinition of struct \"%s\"",name);
        semantic_error(3,node->lineno,msg);
        return;
    }
    FieldList fl=tp->u.structure; 
    while(fl!=NULL)
    {
        FieldList tmp=fl->tail;
        while(tmp!=NULL)
        {
            if(!strcmp(fl->name,tmp->name))
            {
                char msg[100];
                sprintf(msg,"Redefined field \"%s\"",fl->name);
                semantic_error(15,node->lineno,msg);
                return;
            }
            tmp=tmp->tail;
        }
        fl=fl->tail;
    }
}


void Define_Variable(ast node)
{
    DefList dl=construct_Def(node);
    while(dl!=NULL)
    {
        Symbol sym=construct_variable_symbol(dl->name,dl->type);
        if(!trieinsert(sym))
        {
            char msg[100];
            sprintf(msg,"Redefinition of variable \"%s\"",dl->name);
            semantic_error(3,node->lineno,msg);
        }
        dl=dl->tail;
    }
}

void Define_Function(ast node)
{
    FunDec fd=construct_FunDec(node);
    Symbol sym=construct_function_symbol(fd->name,fd);
    if(!trieinsert(sym))
    {
        char msg[100];
        sprintf(msg,"Redefinition of function \"%s\"",fd->name);
        semantic_error(4,node->lineno,msg);
    } 
    VarList arg=fd->args;
    while(arg!=NULL) 
    {
        sym=construct_variable_symbol(arg->name,arg->type);
        trieinsert(sym);
        arg=arg->tail;
    }
}

void return_analysis(ast node)
{
    int lineno=node->lineno;
    node=node->parent;
    Type tp=construct_expression_type(p2(node));
    while(node!=NULL&&strcmp(node->nodetype,"ExtDef")) node=node->parent;
    if(node==NULL) return;
    FunDec fd=construct_FunDec(node);
    Type tp2=fd->type;
    if(!SameType(tp,tp2))
    {
        char msg[100];
        sprintf(msg,"Type mismatched for return.");
        semantic_error(8,lineno,msg);
    }
}

void semantic_analysis(ast node)
{
    if(node==NULL) return;
    if(!strcmp(node->nodetype,"StructSpecifier")) return;
    if(!strcmp(node->nodetype,"Def")) Define_Variable(node);
    else if(!strcmp(node->nodetype,"ExtDef")) ExtDef(node); 
    else if(!strcmp(node->nodetype,"Exp")) expression_semantic_analysis(node);
    else if(!strcmp(node->nodetype,"RETURN")) return_analysis(node);
    semantic_analysis(p1(node));
    semantic_analysis(node->sibling);
} 

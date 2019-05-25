#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "defi.h"

void funcinit()
{
    Symbol symread=malloc(sizeof(struct Symbol_));
    symread->name="read";
    symread->SymbolType=Function;
    symread->u.func=malloc(sizeof(struct FunDec_));
    symread->u.func->type=newint();
    symread->u.func->args=NULL;
    assert(trieinsert(symread));
    Symbol symwrite=malloc(sizeof(struct Symbol_));
    symwrite->name="write";
    symwrite->SymbolType=Function;
    symwrite->u.func=malloc(sizeof(struct FunDec_));
    symwrite->u.func->type=newint();
    symwrite->u.func->args=malloc(sizeof(struct VarList_));
    symwrite->u.func->args->name="x";
    symwrite->u.func->args->type=newint();
    symwrite->u.func->args->tail=NULL;
    assert(trieinsert(symwrite));
}
int labelcnt=0,variablecnt=0,tempcnt=0;
ICVariable newvariable()
{
    variablecnt++;
    ICVariable v=malloc(sizeof(struct ICVariable_));
    v->kind=VAR;
    v->cnt=variablecnt;
    return v;
}
ICVariable newtemp()
{
    labelcnt++;
    ICVariable v=malloc(sizeof(struct ICVariable_));
    v->kind=LAB;
    v->cnt=variablecnt;
    return v;
}
ICVariable newlabel()
{
    labelcnt++;
    ICVariable v=malloc(sizeof(struct ICVariable_));
    v->kind=LAB;
    v->cnt=labelcnt;
    return v;
}
void PrintICVariable(ICVariable v)
{
    switch(v->kind)
    {
        case VAR: {printf("v"); break;}
        case TEMP: {printf("t"); break;}
        case LAB: {printf("label"); break;}
    }
    printf("%d",v->cnt);
}
void print_intermediate_code(InterCode ic)
{
    switch(ic->kind)
    {
        case LABEL:
        {
            break;
        }
        case FUNC:
        {
            break;
        }
        case ASSIGN:
        {
            break;
        }
        case ADD:
        {
            break;
        }
        case SUB:
        {
            break;
        }
        case MUL:
        {
            break;
        }
        case DIVI:
        {
            break;
        }
        case ADDR:
        {
            break;
        }
        case RVAL:
        {
            break;
        }
        case LVAL:
        {
            break;
        }
        case GOTO:
        {
            break;
        }
        case IFGOTO:
        {
            break;
        }
        case RET:
        {
            break;
        }
        case DEC:
        {
            break;
        }
        case ARG:
        {
            break;
        }
        case CALL:
        {
            break;
        }
        case PARAM:
        {
            break;
        }
        case READ:
        {
            break;
        }
        case WRITE:
        {
            break;
        }
        default: break;
    }
}

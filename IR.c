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
void print_ICVariable(ICVariable v)
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
            printf("LABEL ");
            print_ICVariable(ic->u.unary.op1);
            printf(" :\n");
            break;
        }
        case FUNC:
        {
            printf("FUNCTION %s :\n",ic->u.fundec.funcname);
            break;
        }
        case ASSIGN:
        {
            print_ICVariable(ic->u.binary.op1);
            printf(" := ");
            print_ICVariable(ic->u.binary.op2);
            printf("\n");
            break;
        }
        case ADD:
        {
            print_ICVariable(ic->u.ternary.op1);
            printf(" := ");
            print_ICVariable(ic->u.ternary.op2);
            printf(" + ");
            print_ICVariable(ic->u.ternary.op3);
            printf("\n");
            break;
        }
        case SUB:
        {
            print_ICVariable(ic->u.ternary.op1);
            printf(" := ");
            print_ICVariable(ic->u.ternary.op2);
            printf(" - ");
            print_ICVariable(ic->u.ternary.op3);
            printf("\n");
            break;
        }
        case MUL:
        {
            print_ICVariable(ic->u.ternary.op1);
            printf(" := ");
            print_ICVariable(ic->u.ternary.op2);
            printf(" * ");
            print_ICVariable(ic->u.ternary.op3);
            printf("\n");
            break;
        }
        case DIVI:
        {
            print_ICVariable(ic->u.ternary.op1);
            printf(" := ");
            print_ICVariable(ic->u.ternary.op2);
            printf(" / ");
            print_ICVariable(ic->u.ternary.op3);
            printf("\n");
            break;
        }
        case ADDR:
        {
            print_ICVariable(ic->u.binary.op1);
            printf(" := &");
            print_ICVariable(ic->u.binary.op2);
            printf("\n");
            break;
        }
        case RVAL:
        {
            print_ICVariable(ic->u.binary.op1);
            printf(" := *");
            print_ICVariable(ic->u.binary.op2);
            printf("\n");
            break;
        }
        case LVAL:
        {
            printf("*");
            print_ICVariable(ic->u.binary.op1);
            printf(" := ");
            print_ICVariable(ic->u.binary.op2);
            printf("\n");
            break;
        }
        case GOTO:
        {
            printf("GOTO ");
            print_ICVariable(ic->u.unary.op1);
            printf("\n");
            break;
        }
        case IFGOTO:
        {
            printf("IF ");
            print_ICVariable(ic->u.ig.op1);
            switch(ic->u.ig.relop)
            {
                case EQ_: {printf(" == "); break;}
                case LE_: {printf(" < "); break;}
                case GE_: {printf(" > "); break;}
                case LEQ_: {printf(" <= "); break;}
                case GEQ_: {printf(" >= "); break;}
                case NEQ_: {printf(" != "); break;}
            }
            print_ICVariable(ic->u.ig.op2);
            printf(" GOTO ");
            print_ICVariable(ic->u.ig.op3);
            printf("\n");
            break;
        }
        case RET:
        {
            printf("RETURN ");
            print_ICVariable(ic->u.unary.op1);
            printf("\n");
            break;
        }
        case DEC:
        {
            printf("DEC ");
            print_ICVariable(ic->u.dec.op1);
            printf(" [%d] ",ic->u.dec.sz);
            printf("\n");
            break;
        }
        case ARG:
        {
            printf("ARG ");
            print_ICVariable(ic->u.unary.op1);
            printf("\n");
            break;
        }
        case CALL:
        {
            print_ICVariable(ic->u.funcall.op1);
            printf(" := CALL %s\n",ic->u.funcall.funcname);
            break;
        }
        case PARAM:
        {
            printf("PARAM ");
            print_ICVariable(ic->u.unary.op1);
            printf("\n");
            break;
        }
        case READ:
        {
            printf("READ ");
            print_ICVariable(ic->u.unary.op1);
            printf("\n");
            break;
        }
        case WRITE:
        {
            printf("WRITE ");
            print_ICVariable(ic->u.unary.op1);
            printf("\n");
            break;
        }
        default: break;
    }
}

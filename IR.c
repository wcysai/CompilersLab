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
ICVariable direct_number(int d)
{
    ICVariable v=malloc(sizeof(struct ICVariable_));
    v->kind=DNUM;
    v->cnt=d;
    return v;
}
InterCode newcode()
{
    InterCode code=malloc(sizeof(struct InterCode_));
    code->prev=code->next=NULL;
    return code;
}
void print_ICVariable(ICVariable v)
{
    switch(v->kind)
    {
        case VAR: {printf("v"); break;}
        case TEMP: {printf("t"); break;}
        case LAB: {printf("label"); break;}
        case DNUM: {printf("#%d",v->cnt); return;}
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
void print_intermediate_program(InterCode pt)
{
    while(pt!=NULL)
    {
        print_intermediate_code(pt);
        pt=pt->next;
    }
}
InterCode bind_code(InterCode code1,InterCode code2)
{
    InterCode ptr=code1;
    while(ptr->next!=NULL) ptr=ptr->next;
    ptr->next=code2;
    code2->prev=ptr;
    return code1;
}
InterCode translate_Exp(ast node,ICVariable v)
{
    switch(node->opnum)
    {
        case 1:
        {
            ICVariable v2=find_icv(p1(node)->nodename);
            InterCode code=newcode();
            code->kind=ASSIGN;
            code->u.binary.op1=v;
            code->u.binary.op2=v2;
            return code;
        }
        case 2:
        {
            int d=p1(node)->val.intval;
            InterCode code=newcode();
            code->kind=ASSIGN;
            code->u.binary.op1=v;
            code->u.binary.op2=direct_number(d);
            return code;
        }
        case 4:
        {

        }
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        {

        }
        case 13:
        {
            ICVariable t1=newtemp(),t2=newtemp();
            InterCode code1=translate_Exp(p1(node),t1),code2=translate_Exp(p3(node),t2);
            InterCode code3=newcode();
            code3->kind=ADD;
            code3->u.ternary.op1=v;
            code3->u.ternary.op2=t1;
            code3->u.ternary.op3=t2;
            return bind_code(code1,bind_code(code2,code3));
        }
        case 14:
        {
            ICVariable t1=newtemp(),t2=newtemp();
            InterCode code1=translate_Exp(p1(node),t1),code2=translate_Exp(p3(node),t2);
            InterCode code3=newcode();
            code3->kind=SUB;
            code3->u.ternary.op1=v;
            code3->u.ternary.op2=t1;
            code3->u.ternary.op3=t2;
            return bind_code(code1,bind_code(code2,code3));
        }
        case 15:
        {
            ICVariable t1=newtemp(),t2=newtemp();
            InterCode code1=translate_Exp(p1(node),t1),code2=translate_Exp(p3(node),t2);
            InterCode code3=newcode();
            code3->kind=MUL;
            code3->u.ternary.op1=v;
            code3->u.ternary.op2=t1;
            code3->u.ternary.op3=t2;
            return bind_code(code1,bind_code(code2,code3));
        }
        case 16:
        {
            ICVariable t1=newtemp(),t2=newtemp();
            InterCode code1=translate_Exp(p1(node),t1),code2=translate_Exp(p3(node),t2);
            InterCode code3=newcode();
            code3->kind=DIVI;
            code3->u.ternary.op1=v;
            code3->u.ternary.op2=t1;
            code3->u.ternary.op3=t2;
            return bind_code(code1,bind_code(code2,code3));
        }
        case 17:
        {
            return translate_Exp(p2(node),v);
        }
        case 18:
        {
            ICVariable t1=newtemp();
            InterCode code1=translate_Exp(p2(node),t1);
            InterCode code2=newcode();
            code2->kind=SUB;
            code2->u.ternary.op1=v;
            code2->u.ternary.op2=direct_number(0);
            code2->u.ternary.op3=t1;
            return bind_code(code1,code2);
        }
        default: return NULL;
    }
}
InterCode translate_Cond(ast node,ICVariable label_true,ICVariable label_false)
{
    switch(node->opnum)
    {
        case 5:
        {
            ICVariable label1=newlabel();
            InterCode code1=translate_Cond(p1(node),label1,label_false);
            InterCode code2=translate_Cond(p3(node),label_true,label_false);
            InterCode code3=newcode();
            code3->kind=LABEL;
            code3->u.unary.op1=label1;
            return bind_code(code1,bind_code(code3,code2));
        }
        case 6:
        {
            ICVariable label1=newlabel();
            InterCode code1=translate_Cond(p1(node),label_true,label1);
            InterCode code2=translate_Cond(p3(node),label_true,label_false);
            InterCode code3=newcode();
            code3->kind=LABEL;
            code3->u.unary.op1=label1;
            return bind_code(code1,bind_code(code3,code2));
        }
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        {
            ICVariable t1=newtemp(),t2=newtemp();
            InterCode code1=translate_Exp(p1(node),t1),code2=translate_Exp(p3(node),t2);
            InterCode code3=newcode();
            code3->kind=IFGOTO;
            code3->u.ig.op1=t1; code3->u.ig.op2=t2; code3->u.ig.op3=label_true;
            code3->u.ig.relop=node->opnum-RELOPMAGIC;
            InterCode code4=newcode();
            code4->kind=GOTO;
            code4->u.unary.op1=label_false;
            return bind_code(bind_code(code1,code2),bind_code(code3,code4));    
        }
        case 19:
        {
            return translate_Cond(p2(node),label_false,label_true);
        }
        default: break;
    }
}

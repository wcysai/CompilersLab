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
            if(ic->u.unary.op1==NULL) return;
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
            if(ic->u.binary.op1==NULL||ic->u.binary.op2==NULL) return;
            print_ICVariable(ic->u.binary.op1);
            printf(" := ");
            print_ICVariable(ic->u.binary.op2);
            printf("\n");
            break;
        }
        case ADD:
        {
            if(ic->u.ternary.op1==NULL||ic->u.ternary.op2==NULL||ic->u.ternary.op3==NULL) return;
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
            if(ic->u.ternary.op1==NULL||ic->u.ternary.op2==NULL||ic->u.ternary.op3==NULL) return;
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
            if(ic->u.ternary.op1==NULL||ic->u.ternary.op2==NULL||ic->u.ternary.op3==NULL) return;
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
            if(ic->u.ternary.op1==NULL||ic->u.ternary.op2==NULL||ic->u.ternary.op3==NULL) return;
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
            if(ic->u.binary.op1==NULL||ic->u.binary.op2==NULL) return;
            print_ICVariable(ic->u.binary.op1);
            printf(" := &");
            print_ICVariable(ic->u.binary.op2);
            printf("\n");
            break;
        }
        case RVAL:
        {
            if(ic->u.binary.op1==NULL||ic->u.binary.op2==NULL) return;
            print_ICVariable(ic->u.binary.op1);
            printf(" := *");
            print_ICVariable(ic->u.binary.op2);
            printf("\n");
            break;
        }
        case LVAL:
        {
            if(ic->u.binary.op1==NULL||ic->u.binary.op2==NULL) return;
            printf("*");
            print_ICVariable(ic->u.binary.op1);
            printf(" := ");
            print_ICVariable(ic->u.binary.op2);
            printf("\n");
            break;
        }
        case GOTO:
        {
            if(ic->u.unary.op1==NULL) return;
            printf("GOTO ");
            print_ICVariable(ic->u.unary.op1);
            printf("\n");
            break;
        }
        case IFGOTO:
        {
            if(ic->u.ig.op1==NULL||ic->u.ig.op2==NULL||ic->u.ig.op3==NULL) return;
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
            if(ic->u.unary.op1==NULL) return;
            printf("RETURN ");
            print_ICVariable(ic->u.unary.op1);
            printf("\n");
            break;
        }
        case DEC:
        {
            if(ic->u.dec.op1==NULL) return;
            printf("DEC ");
            print_ICVariable(ic->u.dec.op1);
            printf(" [%d] ",ic->u.dec.sz);
            printf("\n");
            break;
        }
        case ARG:
        {
            if(ic->u.unary.op1==NULL) return;
            printf("ARG ");
            print_ICVariable(ic->u.unary.op1);
            printf("\n");
            break;
        }
        case CALL:
        {
            if(ic->u.funcall.op1==NULL) return;
            print_ICVariable(ic->u.funcall.op1);
            printf(" := CALL %s\n",ic->u.funcall.funcname);
            break;
        }
        case PARAM:
        {
            if(ic->u.unary.op1==NULL) return;
            printf("PARAM ");
            print_ICVariable(ic->u.unary.op1);
            printf("\n");
            break;
        }
        case READ:
        {
            if(ic->u.unary.op1==NULL) return;
            printf("READ ");
            print_ICVariable(ic->u.unary.op1);
            printf("\n");
            break;
        }
        case WRITE:
        {
            if(ic->u.unary.op1==NULL) return;
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
InterCode bind_code3(InterCode code1,InterCode code2,InterCode code3)
{
    return bind_code(code1,bind_code(code2,code3));
}
InterCode bind_code4(InterCode code1,InterCode code2,InterCode code3,InterCode code4)
{
    return bind_code(code1,bind_code(code2,bind_code(code3,code4)));
}
InterCode newLABEL(ICVariable v)
{
    InterCode code=newcode();
    code->kind=LABEL;
    code->u.unary.op1=v;
    return code;
}
InterCode newGOTO(ICVariable v)
{
    InterCode code=newcode();
    code->kind=GOTO;
    code->u.unary.op1=v;
    return code;
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
        case 19:
        {
            ICVariable label1=newlabel(),label2=newlabel();
            InterCode code0=newcode(),code2=newLABEL(label1),code3=newcode();
            code0->kind=ASSIGN;
            code0->u.binary.op1=v; code0->u.binary.op2=direct_number(0);
            code3->kind=ASSIGN;
            code3->u.binary.op1=v; code0->u.binary.op2=direct_number(1);
            code2=bind_code(code2,code3);
            code3=newLABEL(label2); 
            InterCode code1=translate_Cond(node,label1,label2);
            return bind_code4(code0,code1,code2,code3);
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
            return bind_code3(code1,code2,code3);
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
            return bind_code3(code1,code2,code3);
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
            return bind_code3(code1,code2,code3);
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
            return bind_code3(code1,code2,code3);
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
            InterCode code3=newLABEL(label1);
            return bind_code3(code1,code3,code2);
        }
        case 6:
        {
            ICVariable label1=newlabel();
            InterCode code1=translate_Cond(p1(node),label_true,label1);
            InterCode code2=translate_Cond(p3(node),label_true,label_false);
            InterCode code3=newLABEL(label1);
            return bind_code3(code1,code3,code2);
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
            InterCode code4=newGOTO(label_false);
            return bind_code4(code1,code2,code3,code4);    
        }
        case 19:
        {
            return translate_Cond(p2(node),label_false,label_true);
        }
        default: break;
    }
}
InterCode translate_Stmt(ast node)
{
    switch(node->opnum)
    {
        case 1:
        {
            return translate_Exp(p1(node),NULL);
        }
        case 2:
        {

        }
        case 3:
        {
            ICVariable t1=newtemp();
            InterCode code1=translate_Exp(p2(node),t1);
            InterCode code2=newcode();
            code2->kind=RET;
            code2->u.unary.op1=t1;
            return bind_code(code1,code2);
        }
        case 4:
        {
            ICVariable label1=newlabel(),label2=newlabel();
            InterCode code1=translate_Cond(p3(node),label1,label2);
            InterCode code3=translate_Stmt(p5(node));
            InterCode code2=newLABEL(label1);
            InterCode code4=newLABEL(label2);
            return bind_code4(code1,code2,code3,code4);   
        }
        case 5:
        {
            ICVariable label1=newlabel(),label2=newlabel(),label3=newlabel();
            InterCode code1=translate_Cond(p3(node),label1,label2);
            InterCode code3=translate_Stmt(p5(node));
            InterCode code6=translate_Stmt(p7(node));
            InterCode code2=newLABEL(label1),code4=newGOTO(label3),code5=newLABEL(label2),code7=newLABEL(label3);
            code1=bind_code4(code1,code2,code3,code4);
            return bind_code4(code1,code5,code6,code7);
        }
        case 6:
        {
            ICVariable label1=newlabel(),label2=newlabel(),label3=newlabel();
            InterCode code2=translate_Cond(p3(node),label2,label3);
            InterCode code4=translate_Stmt(p5(node));
            InterCode code1=newLABEL(label1),code3=newLABEL(label2),code5=newGOTO(label1),code6=newLABEL(label3);
            code1=bind_code4(code1,code2,code3,code4);
            return bind_code3(code1,code5,code6);
        }
        default: return NULL;
    }
}

#include "defi.h"
#include <stdlib.h>
//16
MIPSOp newmipsop()
{
    MIPSOp op=malloc(sizeof(struct MIPSOp_));
    return op;
}
MIPSCode newmipscode()
{
    MIPSCode code=malloc(sizeof(struct MIPSCode_));
    code->prev=code->next=NULL;
    return code;
}
MIPSCode bind_mipscode(MIPSCode code1,MIPSCode code2)
{
    if(code1==NULL) return code2;
    if(code2==NULL) return code1;
    MIPSCode ptr=code1;
    while(ptr->next!=NULL) ptr=ptr->next;
    ptr->next=code2;
    code2->prev=ptr;
    return code1;
}
MIPSCode bind_mipscode3(MIPSCode code1,MIPSCode code2,MIPSCode code3)
{
    return bind_mipscode(code1,bind_mipscode(code2,code3));
}
MIPSCode bind_mipscode4(MIPSCode code1,MIPSCode code2,MIPSCode code3,MIPSCode code4)
{
    return bind_mipscode(code1,bind_mipscode(code2,bind_mipscode(code3,code4)));
}
MIPSCode construct_mips_label(ICVariable op)
{
    MIPSCode code=newmipscode();
    code->kind=MIPS_LABEL;
    return code;
}/*
MIPSOp construct_mips_op(ICVariable icv)
{
    switch(icv->kind)
    {
        case VAR:
        case TEMP:
        {
            MIPSOp op=newmipsop();
            op->kind=MOP_STA;
            op->
        }
    }
}
MIPSCode translate_to_mipscode(InterCode ic)
{
    switch(ic->kind)
    {
        case LABEL:
        {
            MIPSCode code=construct_mips_label(ic->u.unary.op1);
            return code;
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
        case SUB: 
        case MUL:
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
            printf(" %d ",ic->u.dec.sz);
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
            if(ic->u.funcall.op1==NULL) ic->u.funcall.op1=newtemp();
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
}*/

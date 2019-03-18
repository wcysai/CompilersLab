%{
    #include<stdio.h>
%}
/* types */
%union
{
    int type_int;
    float type_float;
}

/* tokens */
%token <type_int> INT 
%token <type_float> FLOAT 
%token ID
%token ADD SUB MUL DIV
%token SEMI COMMA
%token ASSIGNOP
%token LE GE LEQ GEQ EQ NEQ
%token PLUS MINUS STAR DIV
%token AND OR
%token DOT NOT
%token TYPE
%token LP RP LB RB LC RC
%token STRUCT RETURN IF ELSE WHILE 
%%
Program: ExtDefList
    ;
ExtDefList: ExtDef ExtDefList
    |
    ;
ExtDef: Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CopmSt
    ;
ExtDecList: VarDec
    | VarDec COMMA ExtDecList
Specifier: TYPE
    | StructSpecifier
    ;
StructSpecifier: STRUCT OptTag LC DefList
    | STRUCT Tag
    ;
OptTag: ID
    |
    ;
Tag: ID
    ;
VarDec: ID
    | VarDec LB INT RB
    ;
FunDec: ID LP VarList RP
    | ID LP RP
    ;
VarList: ParamDec COMMA VarList
    | ParamDec
    ;
ParamDec: Specifier VarDec
    ;
CompSt: LC DefList StmtList RC
    ;
StmtList: Stmt StmtList
    |
    ;
Stmt: Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
DefList: Def DefList
    | 
    ;
Def: Specifier DecList SEMI
    ;
DecList: Dec
    | Dec COMMA DecList
    ;
Dec: VarDec
    | VarDec ASSIGNOP Exp

Exp: ID
    | INT {$$=$1;}
    | FLOAT {$$=$1;}
    | Exp ASSIGNOP Exp {$$=($1==$3);}
    | Exp AND Exp {$$=($1&&$3);}
    | Exp OR Exp {$$=($1||$3);}
    | Exp LE Exp {$$=($1<$3);}
    | Exp GE Exp {$$=($1>$3);}
    | Exp GEQ Exp {$$=($1>=$3);} 
    | Exp LEQ Exp {$$=($1<=$3);}
    | Exp EQ Exp {$$=($1==$3);}
    | Exp NEQ Exp {$$=($1!=$3);}
    | Exp PLUS Exp {$$=$1+$3;}
    | Exp MINUS Exp {$$=$1-$3;}
    | Exp STAR Exp {$$=$1*$3;}
    | Exp DIV Exp {$$=$1/$3;}
    | LP Exp RP {$$=$2;}
    | MINUS Exp {$$=-$2;}
    | NOT Exp {$$=!$2;}
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    ;
Args: Exp COMMA Args
    | Exp
    ;
%%
#include"lex.yy.c"
int main()
{
    yyparse();
}
yyerror(char *msg)
{
    fprintf(stderr,"error: %s\n",msg);
}

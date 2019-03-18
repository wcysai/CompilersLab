%{
    #include<stdio.h>
%}

/* declared tokens */
%token INT
%token ADD SUB MUL DIV
%%
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
    | INT
    | FLOAT
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

%{
    #include<stdio.h>
    #include<math.h>
    #include<stdbool.h>
    #include "defi.h"
    #include"lex.yy.c"
%}
/* types */
%union
{
    struct ast *a;
}
%type <a> Exp 
%type <a> INT FLOAT
/* tokens */
%token ID
%token INT FLOAT
%token SEMI COMMA
%token ASSIGNOP
%token LE GE LEQ GEQ EQ NEQ
%token PLUS MINUS STAR DIV
%token AND OR
%token DOT NOT
%token TYPE
%token LP RP LB RB LC RC
%token STRUCT RETURN IF ELSE WHILE 
%right ASSIGNOP
%left OR
%left AND
%left LE GE LEQ GEQ EQ NEQ
%left PLUS MINUS
%left STAR DIV
%right NOT UMINUS
%left LP RP LB RB DOT
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%%
Program: ExtDefList
    ;
ExtDefList: ExtDef ExtDefList
    |
    ;
ExtDef: Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
    ;
ExtDecList: VarDec
    | VarDec COMMA ExtDecList
Specifier: TYPE
    | StructSpecifier
    ;
StructSpecifier: STRUCT OptTag LC DefList RC
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
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
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

Exp: ID {
        $$=newast("ID")
        $$->nodetype="ID";
    }
    | INT  {
        $$=$1; $$->lineno=@$.first_line;
        $$->nodetype="INT";
    }
    | FLOAT {
        $$=$1; $$.lineno=@$.first_line;
        $$->nodetype="FLOAT";
    } 
    | Exp ASSIGNOP Exp {
        $$=newast("ASSIGNOP",@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2);
    }
    | Exp AND Exp {$$=newast(AND,$1,$3);}
    | Exp OR Exp {$$=newast(OR,$1,$3);}
    | Exp LE Exp {$$=newast(LE,$1,$3);}
    | Exp GE Exp {$$=newast(GE,$1,$3);}
    | Exp GEQ Exp {$$=newast(GEQ,$1,$3);} 
    | Exp LEQ Exp {$$=newast(LEQ,$1,$3);}
    | Exp EQ Exp {$$=newast(EQ,$1,$3);}
    | Exp NEQ Exp {$$=newast(LEQ,$1,$3);}
    | Exp PLUS Exp {$$=newast(PLUS,$1,$3);}
    | Exp MINUS Exp {$$=newast(MINUS,$1,$3);}
    | Exp STAR Exp {$$=newast(STAR,$1,$3);}
    | Exp DIV Exp {$$=newast(DIV,$1,$3);}
    | LP Exp RP {$$=$2;}
    | MINUS Exp {$$=newast(MINUS,$2,NULL);} %prec UMINUS
    | NOT Exp {$$=newast(NOT,$2,NULL);}
    | ID LP Args RP 
    | ID LP RP 
    | Exp LB Exp RB
    | Exp DOT ID
    ;
Args: Exp COMMA Args
    | Exp
    ;
%%
int main(int argc, char** argv)
{
    if(argc<=1) return 1;
    FILE* f=fopen(argv[1],"r");
    if(!f)
    {
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    if(yyparse()) puts("done");
}


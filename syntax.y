%{
    #include<stdio.h>
%}

/* declared tokens */
%token INT
%token ADD SUB MUL DIV
%%
Exp: ID
   | INT
   | FLOAT
   | Exp ASSIGNOP Exp {$$=($1==$3);}
   | Exp AND Exp {$$=($1&&$3);}
   | Exp OR Exp {$$=($1||$3);}
   | Exp PLUS Exp {$$=$1+$3;}
   | Exp MINUS Exp {$$=$1-$3;}
   | Exp STAR Exp {$$=$1*$3;}
   | Exp DIV Exp {$$=$1/$3;}
   | LP Exp RP {$$=$2;}
   | MINUS Exp {$$=-$2;}
   | NOT Exp {$$=!$2;}
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

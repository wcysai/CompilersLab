%{
    #include<math.h>
    #include "syntax.tab.h"
    int yycolumn=1;
#define YY_USER_ACTION\
    yylloc.first_line=yylloc.last_line=yylineno;\
    yylloc.first_column=yycolumn;\
    yylloc.last_column=yycolumn+yyleng-1;\
    yycolumn+=yyleng;
    #define YYSTYPE struct ast*
    extern int no_syntax_error;
%}
%option noyywrap nodefault yylineno
octonary_digit [0-7]
non_zero_octonary_digit [1-7]
hexadecimal_digit [0-9]|[A-F]|[a-f]
non_zero_hexadecimal_digit [1-9]|[A-F]|[a-f]
digit [0-9]
non_zero_digit [1-9]
zero 0
letter [_a-zA-Z]
ID {letter}({letter}|{digit})*
DECIMAL_INT {non_zero_digit}{digit}*
OCTONARY_INT 0{non_zero_octonary_digit}{octonary_digit}*
HEXADECIMAL_INT (0x|0X){non_zero_hexadecimal_digit}{hexadecimal_digit}*
NON_ZERO_INT {DECIMAL_INT}|{OCTONARY_INT}|{HEXADECIMAL_INT}
INT {NON_ZERO_INT}|0
EXPONENTIAL_FORM [E|e][+|-]?{digit}+
FLOAT {digit}*(\.{digit}*|(\.{digit}*{EXPONENTIAL_FORM}))
LZ 0{NON_ZERO_INT}
LINE_COMMENT "//".*
BLOCK_COMMENT_START "/*"
BLOCK_COMMENT_END "*/"
COMMENTED_LINE {LINE_COMMENT}.*
COMMENTED_BLOCK {BLOCK_COMMENT_START}([^\*]|(\*+[^*/]))*{BLOCK_COMMENT_END}
WHITESPACE [ \t]
SEMI ;
COMMA ,
ASSIGNOP =
LE <
GE >
LEQ <=
GEQ >=
EQ ==
NEQ !=
PLUS \+
MINUS -
STAR \*
DIV \/
AND &&
OR \|\|
DOT \.
NOT !
TYPE "int"|"float"
LP "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
OTHERWISE . 
NEWLINE \n
%%
{WHITESPACE} {}
{NEWLINE} {yycolumn=1;}
{SEMI} {yylval=newast("SEMI",0,yylineno,""); return SEMI;} 
{COMMA} {yylval=newast("COMMA",0,yylineno,""); return COMMA;} 
{LE} {yylval=newast("LE",0,yylineno,""); return LE;} 
{GE} {yylval=newast("GE",0,yylineno,""); return GE;} 
{LEQ} {yylval=newast("LEQ",0,yylineno,""); return LEQ;} 
{GEQ} {yylval=newast("GEQ",0,yylineno,""); return GEQ;} 
{EQ} {yylval=newast("EQ",0,yylineno,""); return EQ;} 
{NEQ} {yylval=newast("NEQ",0,yylineno,""); return NEQ;} 
{ASSIGNOP} {yylval=newast("ASSIGNOP",0,yylineno,""); return ASSIGNOP;} 
{PLUS} {yylval=newast("PLUS",0,yylineno,""); return PLUS;} 
{MINUS} {yylval=newast("MINUS",0,yylineno,""); return MINUS;} 
{STAR} {yylval=newast("STAR",0,yylineno,""); return STAR;} 
{DIV} {yylval=newast("DIV",0,yylineno,""); return DIV;} 
{AND} {yylval=newast("AND",0,yylineno,""); return AND;} 
{OR} {yylval=newast("OR",0,yylineno,""); return OR;} 
{DOT} {yylval=newast("DOT",0,yylineno,""); return DOT;} 
{NOT} {yylval=newast("NOT",0,yylineno,""); return NOT;} 
{LP} {yylval=newast("LP",0,yylineno,""); return LP;}
{RP} {yylval=newast("RP",0,yylineno,""); return RP;}
{LB} {yylval=newast("LB",0,yylineno,""); return LB;}
{RB} {yylval=newast("RB",0,yylineno,""); return RB;}
{LC} {yylval=newast("LC",0,yylineno,""); return LC;}
{RC} {yylval=newast("RC",0,yylineno,""); return RC;} 
{STRUCT} {yylval=newast("STRUCT",0,yylineno,""); return STRUCT;} 
{RETURN} {yylval=newast("RETURN",0,yylineno,""); return RETURN;} 
{IF} {yylval=newast("IF",0,yylineno,""); return IF;} 
{ELSE} {yylval=newast("ELSE",0,yylineno,""); return ELSE;} 
{WHILE} {yylval=newast("WHILE",0,yylineno,""); return WHILE;} 
{TYPE} {yylval=newast("TYPE",0,yylineno,""); if(*yytext=='i') yylval->nodename="int"; else yylval->nodename="float"; return TYPE;} 
{ID} {yylval=newast("ID",0,yylineno,""); yylval->nodename=malloc(sizeof(char*));strcpy(yylval->nodename,yytext); return ID;}
{INT} {
    int x;
    if(strncmp(yytext,"0x",2)==0)
    {
        x=0;
        yytext+=2;
        while(*yytext!='\0')
        {
            if(*yytext>='0'&&*yytext<='9') x=x*16+(*yytext-'0');
            else if(*yytext>='a'&&*yytext<='f') x=x*16+(10+*yytext-'a');
            else x=x*16+(10+*yytext-'A');
            yytext++;
        }
    }
    else if(strncmp(yytext,"0",1)==0) 
    {
        int x=0;
        yytext++;
        while(*yytext!='\0')
        {
            x=x*8+(*yytext-'0');
            yytext++;
        }
    }
    else 
    {
        x=atoi(yytext);
    }
    yylval=newast("INT",0,yylineno,"");
    yylval->intval=x;
    return INT;
    }
{LZ} {printf("Error type A at Line %d: illegal leading zeroes detected\n",yylineno); no_syntax_error=0;}
{FLOAT} {
    double x=0.0;
    while(*yytext!='.')
    {
        x=x*10.0+(*yytext-'0');
        yytext++;
    }
    yytext++;
    if(*yytext>='0'&&*yytext<='9')
    {
        double deci=0.1;
        do
        {
            x+=deci*(*yytext-'0');
            yytext++; deci*=0.1;
        }while(*yytext>='0'&&*yytext<='9');
    }
    if(*yytext=='e'||*yytext=='E')
    {
        yytext++;
        bool f=true;
        if(*yytext=='-') {f=false; yytext++;}
        int base=0;
        do
        {
            base=base*10+(*yytext-'0');
            yytext++;
        }while(*yytext!='\0');
        if(!f) base=-base;
        x=x*pow(10.0,base);
    }
    else if(*yytext=='\0');   
    yylval=newast("FLOAT",0,yylineno,"");
    yylval->doubleval=x;
    return FLOAT;
}
{COMMENTED_LINE} {}
{COMMENTED_BLOCK} {}
{OTHERWISE} {printf("Error type A at Line %d: Mysterious character \'%s\'\n",yylineno,yytext); no_syntax_error=0;}
%%
int yyerror(const char *msg) 
{
    fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, msg);
    no_syntax_error=0;
    return 1;
}


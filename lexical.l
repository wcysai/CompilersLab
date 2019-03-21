%{
    #include<math.h>
    #include "syntax.tab.h"
    int yycolumn=1;
#define YY_USER_ACTION\
    yylloc.first_line=yylloc.last_line=yylineno;\
    yylloc.first_column=yycolumn;\
    yylloc.last_column=yycolumn+yyleng-1;\
    yycolumn+=yyleng;
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
BLOCK_COMMENT_START \/\*
BLOCK_COMMENT_END \*\/
COMMENTED_LINE {LINE_COMMENT}.*
COMMENTED_BLOCK {BLOCK_COMMENT_START}([^*]|(\*+[^*/]))*{BLOCK_COMMENT_END}
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
TYPE (int|float)\ +
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
{SEMI} {return SEMI;}
{COMMA} {return COMMA;}
{LE} {return LE;}
{GE} {return GE;}
{LEQ} {return LEQ;}
{GEQ} {return GEQ;}
{EQ} {return EQ;}
{NEQ} {return NEQ;}
{ASSIGNOP} {puts("ASSIGNOP"); return ASSIGNOP;}
{PLUS} {return PLUS;}
{MINUS} {return MINUS;}
{STAR} {return STAR;}
{DIV} {return DIV;}
{AND} {return AND;}
{OR} {return OR;}
{DOT} {return DOT;}
{NOT} {return NOT;}
{LP} {return LP;}
{RP} {return RP;}
{LB} {return LB;}
{RB} {return RB;}
{LC} {return LC;}
{RC} {return RC;}
{STRUCT} {return STRUCT;}
{RETURN} {return RETURN;}
{IF} {return IF;}
{ELSE} {return ELSE;}
{WHILE} {return WHILE;}
{TYPE} {printf("TYPE: %s\n",yytext); return TYPE;}
{ID} {printf("ID: %s\n",yytext); return ID;}
{INT} {
    if(strncmp(yytext,"0x",2)==0)
    {
        int x=0;
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
        printf("INT: %d\n",x);
    }
    else 
    {
        printf("INT: %d\n",atoi(yytext)); 
    }
    return INT;
    }
{LZ} {printf("Error type A at Line %d: illegal leading zeroes detected\n",yylineno);}
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
        printf("FLOAT: %.10f\n",x);
    }
    else if(*yytext=='\0')
    {
        printf("FLOAT: %.10f\n",x);
    }
    return FLOAT;
}
{COMMENTED_LINE} {printf("commented line\n");}
{COMMENTED_BLOCK} {printf("commented block\n");}
{OTHERWISE} {printf("Error type A at Line %d: Mysterious character \'%s\'\n",yylineno,yytext);}
%%
int yyerror(const char *msg) 
{
    fprintf(stderr, "Error: %s\n", msg);
    return 0;
}


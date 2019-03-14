%{
    #include<math.h>
%}
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
LINE_COMMENT \/\/
BLOCK_COMMENT_START \/\*
BLOCK_COMMENT_END \*\/
COMMENTED_LINE {LINE_COMMENT}.*
COMMENTED_BLOCK {BLOCK_COMMENT_START}([^*]|(\*+[^*/]))*{BLOCK_COMMENT_END}
SEMI ;
COMMA ,
ASSIGNOP =
RELOP > | < | >= | <= | == | !=
PLUS +
MINUS -
STAR \*
DIV \/
AND &&
OR \|\|
DOT \.
NOT !
TYPE (int|float)\ +
LP (
RP )
LB [
RB ]
LC {
RC }
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
OTHERWISE .
%%
{TYPE} {printf("TYPE: %s\n",yytext);}
{ID} {printf("ID: %s\n",yytext);}
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
        printf("INT: %d\n",x);
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
    //yylval=x; return INT;
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
}
{COMMENTED_LINE} {printf("commented line\n");}
{COMMENTED_BLOCK} {printf("commented block\n");}
{OTHERWISE} {printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno,yytext);}
%%
int yywrap() {return 1;}

int main()
{
    while(yylex());
    return 0;
}

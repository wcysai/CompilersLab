octonary_digit [0-7]
non_zero_octonary_digit [1-7]
hexadecimal_digit [0-9]|[A-F]|[a-f]
non_zero_hexadecimal_digit [1-9]|[A-F]|[a-f]
digit [0-9]
non_zero_digit [1-9]
zero 0
letter [_a-zA-Z]
ID {letter}({letter}|{digit})+
DECIMAL_INT {non_zero_digit}{digit}+
OCTONARY_INT 0{non_zero_octonary_digit}{octonary_digit}+
HEXADECIMAL_INT [0x|0X]{non_zero_hexadecimal_digit}{hexadecimal_digit}+
INT {DECIMAL_INT}|{OCTONARY_INT}|{HEXADECIMAL_INT}
EXPONENTIAL_FORM [E|e][+|-]?{digit}+
FLOAT {digit}*\.[0-9]+|{digit}*\.{EXPONENTIAL_FORM}
LZ 0+{INT}
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
TYPE int | float
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
{STRUCT} {puts("struct");}
{ID} {puts("id");}
{INT} {
    if(strncmp(yytext,"0x",2)==0) printf("hexadicmal digit\n");
    else if(strncmp(yytext,"0",1)==0) printf("octonary digit\n");
    else printf("deciaml digit\n"); 
    }
{LZ} {puts("leading zeroes detected");}
{SEMI} {puts("semi");}
{STAR} {puts("STAR");}
{DIV} {puts("DIV");}
{OR} {puts("OR");}
{OTHERWISE} {printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno,yytext);}
%%
int yywrap() {return 1;}

int main()
{
    while(yylex());
    return 0;
}

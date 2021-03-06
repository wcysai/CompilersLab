%{
    #include<stdio.h>
    #include<math.h>
    #include<stdbool.h>
    #include<assert.h>
    #include "defi.h"
    #include"lex.yy.c"
    ast root;    
    int no_syntax_error;
%}
/* types */
%define api.value.type {ast}
/*%type <a> Exp 
%type <a> Program ExtDefList ExtDef ExtDecList
%type <a> Specifier StructSpecifier Tag OptTag VarDec FunDec
%type <a> VarList ParamDec CompSt StmtList Stmt
%type <a> DefList Def DecList Dec

%type <a> INT FLOAT ID TYPE
%type <a> STRUCT RETURN IF ELSE WHILE */
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
Program: ExtDefList { //1
        $$=newast("Program",1,@$.first_line,"");
        add_child($$,$1); root=$$; 
    }
    ;
ExtDefList: ExtDef ExtDefList { //1
        $$=newast("ExtDefList",1,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2);
    }
    | {$$=newast("Empty",2,@$.first_line,"");} //2
    ;
ExtDef: Specifier ExtDecList SEMI { //1
        $$=newast("ExtDef",1,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2);  add_sibling($2,$3);
    }
    | Specifier SEMI { //2
        $$=newast("ExtDef",2,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2);
    }
    | Specifier FunDec CompSt { //3
        $$=newast("ExtDef",3,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3); 
    }
    ;
ExtDecList: VarDec { //1
        $$=newast("ExtDecList",1,@$.first_line,"");
        add_child($$,$1); 
    }
    | VarDec COMMA ExtDecList { //2
        $$=newast("ExtDecList",2,@$.first_line,"");
        add_child($$,$1);  add_sibling($1,$2); add_sibling($2,$3); 
    }
    ;
Specifier: TYPE { //1
        $$=newast("Specifier",1,@$.first_line,"");
        add_child($$,$1); 
    }
    | StructSpecifier { //2
        $$=newast("Specifier",2,@$.first_line,"");
        add_child($$,$1); 
    }
    ;
StructSpecifier: STRUCT OptTag LC DefList RC{ //1
        $$=newast("StructSpecifier",1,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3); add_sibling($3,$4); add_sibling($4,$5); 
    }
    | STRUCT Tag{ //2
        $$=newast("StructSpecifier",2,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2); 
    }
    ;
OptTag: ID { //1
        $$=newast("OptTag",1,@$.first_line,"");
        add_child($$,$1); 
    }
    | {$$=newast("Empty",2,@$.first_line,"");} //2
    ;
Tag: ID { //1
        $$=newast("Tag",1,@$.first_line,"");
        add_child($$,$1); 
    }
    ;
VarDec: ID{ //1
        $$=newast("VarDec",1,@$.first_line,"");
        add_child($$,$1); 
    }
    | VarDec LB INT RB{ //2
        $$=newast("VarDec",2,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3); add_sibling($3,$4); 
    }
    | error RB
    ;
FunDec: ID LP VarList RP { //1
        $$=newast("FunDec",1,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3); add_sibling($3,$4);
    }
    | ID LP RP { //2
        $$=newast("FunDec",2,@$.first_line,"");
        //printf("found ID: %s\n",$1->nodename);
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    ;
VarList: ParamDec COMMA VarList{ //1
        $$=newast("VarList",1,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3); 
    }
    | ParamDec { //2
        $$=newast("VarList",2,@$.first_line,"");
        add_child($$,$1); 
    }
    ;
ParamDec: Specifier VarDec{ //1
        $$=newast("ParamDec",1,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2); 
    }
    ;
CompSt: LC DefList StmtList RC{ //1
        $$=newast("CompSt",1,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3); add_sibling($3,$4);
    }
    ;
StmtList: Stmt StmtList { //1
        $$=newast("StmtList",1,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2);
    }
    | {$$=newast("Empty",2,@$.first_line,"");} //2
    ;
Stmt: Exp SEMI { //1
        $$=newast("Stmt",1,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2); 
    }
    | CompSt { //2
        $$=newast("Stmt",2,@$.first_line,"");
        add_child($$,$1); 
    }
    | RETURN Exp SEMI { //3
        $$=newast("Stmt",3,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3); 
    }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE { //4
        $$=newast("Stmt",4,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3); add_sibling($3,$4); add_sibling($4,$5); 
    }
    | IF LP Exp RP Stmt ELSE Stmt { //5
        $$=newast("Stmt",5,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3); add_sibling($3,$4); add_sibling($4,$5); add_sibling($5,$6); add_sibling($6,$7); 
    }
    | WHILE LP Exp RP Stmt{ // 6
        $$=newast("Stmt",6,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3); add_sibling($3,$4); add_sibling($4,$5);
    }
    | error SEMI
    | error
    ;

DefList: Def DefList { //1
        $$=newast("DefList",1,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2);
    }
    | {$$=newast("Empty",2,@$.first_line,"");} //2
    ;

Def: Specifier DecList SEMI { //1
        $$=newast("Def",1,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    ;

DecList: Dec { //1
        $$=newast("DecList",1,@$.first_line,"");
        add_child($$,$1);
    }
    | Dec COMMA DecList { //2
        $$=newast("DecList",2,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    ;

Dec: VarDec { //1
        $$=newast("Dec",1,@$.first_line,"");
        add_child($$,$1);
    }
    | VarDec ASSIGNOP Exp { //2
        $$=newast("Dec",2,@$.first_line,"");
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    ;

Exp: ID { //1
        $$=newast("Exp",1,@$.first_line,"");
        add_child($$,$1);
    }
    | INT  { //2
        $$=newast("Exp",2,@$.first_line,"");
        add_child($$,$1);
    }
    | FLOAT { //3
        $$=newast("Exp",3,@$.first_line,"");
        add_child($$,$1);
    } 
    | Exp ASSIGNOP Exp { //4
        $$=newast("Exp",4,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    | Exp AND Exp { //5
        $$=newast("Exp",5,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    | Exp OR Exp {  //6
        $$=newast("Exp",6,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    | Exp LE Exp { //7
        $$=newast("Exp",7,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    | Exp GE Exp { //8
        $$=newast("Exp",8,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    | Exp GEQ Exp { //9
        $$=newast("Exp",9,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    } 
    | Exp LEQ Exp  { //10
        $$=newast("Exp",10,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    | Exp EQ Exp {  //11
        $$=newast("Exp",11,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    | Exp NEQ Exp {  //12
        $$=newast("Exp",12,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    | Exp PLUS Exp { //13
        $$=newast("Exp",13,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    | Exp MINUS Exp { //14
        $$=newast("Exp",14,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    | Exp STAR Exp { //15
        $$=newast("Exp",15,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    | Exp DIV Exp {  //16
        $$=newast("Exp",16,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    | LP Exp RP {  //17
        $$=newast("Exp",17,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    | MINUS Exp {  //18
        $$=newast("Exp",18,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2);
    } %prec UMINUS
    | NOT Exp  { //19
        $$=newast("Exp",19,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2);
    }
    | ID LP Args RP {  //20
        $$=newast("Exp",20,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3); add_sibling($3,$4);
    }
    | ID LP RP { //21
        $$=newast("Exp",21,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    | Exp LB Exp RB{  //22
        $$=newast("Exp",22,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3); add_sibling($3,$4);
    }
    | Exp DOT ID { //23
        $$=newast("Exp",23,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    ;

Args: Exp COMMA Args{ //1
        $$=newast("Args",1,@$.first_line,""); 
        add_child($$,$1); add_sibling($1,$2); add_sibling($2,$3);
    }
    | Exp{ //2
        $$=newast("Args",2,@$.first_line,""); 
        add_child($$,$1);
    }
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
    no_syntax_error=1;
    yylval=malloc(sizeof(struct ast_));
    yyrestart(f);
    trieinit();
    //yydebug=1;
    yyparse();
    //if(no_syntax_error) print_ast(root,0);
    if(no_syntax_error)
    {
        funcinit();
        semantic_analysis(root);
        InterCode IC=translate_Program(root);
        if(argc>2) freopen(argv[2],"w",stdout);
        print_intermediate_program(IC);
    }
}


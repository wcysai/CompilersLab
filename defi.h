#include<string.h>
#include<stdbool.h>
#define p1(x) x->child
#define p2(x) x->child->sibling
#define p3(x) x->child->sibling->sibling
#define p4(x) x->child->sibling->sibling->sibling
#define p5(x) x->child->sibling->sibling->sibling->sibling
extern int yylineno;
int yyerror(const char *msg);
typedef struct ast_* ast;
struct ast_
{
    char *nodetype; //a string that represents what type the node is
    char *nodename; // the name of the node
    int opnum; //operation id, for calculating the value of a node
    union
    {
        double doubleval; //double value for the node
        int intval; //int value for the node
    }val;
    int lineno; //the line where the node presents
    ast child; //child of the current node(if exists)
    ast sibling; //sibling of the current node(if exists)
    ast parent; //parent of the current node(if exists)
};

struct numval
{
    int nodetype;
    double number;
};

char* extract_name(ast node);

void semantic_error(int type,int lineno,const char* msg);


typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef struct Dec_* Dec;
typedef struct DecList_* DecList;
typedef struct ArrayDec_* ArrayDec;
typedef struct Def_* Def;
typedef struct DefList_* DefList;
typedef struct FunDec_* FunDec;
typedef struct VarList_* VarList;

struct Type_
{
    enum{BASIC,ARRAY,STRUCTURE} kind;
    union
    {
        int basic;
        struct{Type elem; int size;} array;
        FieldList structure;
    }u;
};

Type newint();
Type newfloat();

struct FieldList_
{
    char* name; 
    Type type; 
    FieldList tail; 
};

struct ArrayDec_
{
    int size;
    ArrayDec tail;
};

struct Dec_
{
    char* name;
    ArrayDec array;
};

struct DecList_
{
    Dec dec;
    DecList tail;
};

struct DefList_
{
    char* name;
    Type type;
    DefList tail;
};

struct VarList_
{
    char* name;
    Type type;
    VarList tail;
};

struct FunDec_
{
    char* name;
    Type type;
    VarList args;
};

typedef struct Symbol_ * Symbol;
typedef struct Symlist_* Symlist;
typedef struct Trie_* Trie;
struct Symbol_
{
    char *name; //symbol name
    enum {Function,Variable} SymbolType; //function or variable
    //int lineno; // the line where the symbol is defined
    union 
    {
        Type type;
        FunDec func;
    }u;
};


Dec construct_VarDec(ast node);
Dec construct_Dec(ast node);
DecList construct_DecList(ast node);
Type arrrtype(Type tp,ArrayDec arr);
DefList construct_Def(ast node);
DefList construct_DefList(ast node);
VarList construct_VarList(ast node);
VarList construct_Args(ast node);
FunDec construct_FunDec(ast node);
void Define_Variable(ast node);
void Define_Struct(ast node);
void Define_Function(ast node);
void ExtDef(ast node);


bool SameType(Type p,Type q);
bool SameField(FieldList p,FieldList q);
bool SameFunction(FunDec p,FunDec q);
bool SameVarList(VarList p,VarList q);
bool SameSymbol(Symbol p,Symbol q);
bool IsInt(Type p);
bool IsFloat(Type p);
bool IsArray(Type p);
bool IsStruct(Type p);
bool IsVariable(Symbol p);
bool IsFunction(Symbol p);
Type newint();
Type newfloat();
Type construct_type(ast node);
Type construct_basic(ast node);
Type construct_struct(ast node);
Type search_struct(Type tp,char *name);



extern Trie func,var;

int find_id(char x);
void trieinit();
bool trieinsert(Symbol sym);
bool triedelete(Symbol sym);
Symbol Variable_Lookup(char *name);
Symbol Function_Lookup(char *name);
struct Trie_
{
    Trie next[60];
    Symbol sym;
};

//ast function 
ast newast(char *nodetype, int opnum, int lineno,char *nodename);
void add_child(ast node,ast child);
void add_sibling(ast node,ast sibling);
void print_ast(ast node,int tabs);

//symbol&trie functions

Symbol construct_variable_symbol(char *name,Type tp);
Symbol construct_function_symbol(char *name,FunDec fd);
Type construct_expression_type(ast node);
void expression_semantic_analysis(ast node);
void semantic_analysis(ast node);
void return_analysis(ast node);

//IR functions
void funcinit();

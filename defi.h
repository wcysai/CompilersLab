#include<stdbool.h>
#include<string.h>
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
};

struct numval
{
    int nodetype;
    double number;
};

char* extract_name(ast node);


typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef struct Dec_* Dec;
typedef struct DecList_* DecList;
typedef struct ArrayDec_* ArrayDec;
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


bool SameType(Type p,Type q);
bool SameField(FieldList p,FieldList q);
Type construct_type(ast node);
Type construct_basic(ast node);
Type construct_struct(ast node);
FieldList construct_type_list(ast node);


typedef struct Symbol_ * Symbol;
typedef struct Symlist_* Symlist;
typedef struct Trie_* Trie;
struct Symbol_
{
    char *name; //symbol name
    enum {Function,Variable} SymbolType; //function or variable
    Type type;
    int lineno; // the line where the symbol is defined
    ast treenode; //node in the abstract syntax tree 
    Symlist args; //list of arguments of a function
    Symbol temp; //temporary definition that overrides the current one
};

struct Symlist_//arguments of a function
{
    Type type;
    Symlist next; //next argument
};

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
int find_id(char x);
Trie newnode();
void trieinit();
bool trieinsert(Symbol sym);
Symbol triefind(char *name);
bool triedelete(Symbol sym,int type);


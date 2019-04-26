#include<stdbool.h>
#include<string.h>
extern int yylineno;
int yyerror(const char *msg);
struct ast
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
    struct ast *child; //child of the current node(if exists)
    struct ast *sibling; //sibling of the current node(if exists)
};

struct numval
{
    int nodetype;
    double number;
};

enum {Variable,Function,Struct} SymbolType;

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
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

bool SameType(Type p,Type q);
bool SameField(FieldList p,FieldList q);

struct symbol
{
    char *name; //symbol name
    int symboltype; //function, variable or structs
    int valtype; //int or double or void
    double doubleval; //double value for some symbol
    int intval;      //int value for some symbol
    int lineno; // the line where the symbol is defined
    struct ast *treenode; //node in the abstract syntax tree 
    struct symlist *args; //list of arguments of a function
    struct symbol *temp; //temporary definition that overrides the current one
};

struct symlist//arguments of a function
{
    struct symbol *sym; //symbol of the current argument
    struct symlist *next; //next argument
};

struct trie
{
    struct trie* next[60];
    struct symbol *sym;
};

//ast functions
struct ast *newast(char *nodetype, int opnum, int lineno,char *nodename);
void add_child(struct ast* node,struct ast *child);
void add_sibling(struct ast* node,struct ast *sibling);
void print_ast(struct ast* node,int tabs);

//symbol&trie functions
struct symbol *newsymbol(char *name, int symboltype, int valtype, int lineno,char *nodename);
int find_id(char x);
struct trie *newnode();
void trieinit();
bool trieinsert(struct symbol* sym,int type);
struct symbol *triefind(char *name);
bool triedelete(struct symbol *sym,int type);

double eval(struct ast*);

void treefree(struct ast*);

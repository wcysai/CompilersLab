#include<string.h>
#include<stdbool.h>
#define p1(x) x->child
#define p2(x) x->child->sibling
#define p3(x) x->child->sibling->sibling
#define p4(x) x->child->sibling->sibling->sibling
#define p5(x) x->child->sibling->sibling->sibling->sibling
#define p6(x) x->child->sibling->sibling->sibling->sibling->sibling
#define p7(x) x->child->sibling->sibling->sibling->sibling->sibling->sibling
#define RELOPMAGIC 7
#define MAXNAMELEN 32
typedef long long ll;
typedef unsigned long long ull;
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
    char name[MAXNAMELEN]; 
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
    char name[MAXNAMELEN];
    ArrayDec array;
};

struct DecList_
{
    Dec dec;
    DecList tail;
};

struct DefList_
{
    char name[MAXNAMELEN];
    Type type;
    DefList tail;
};

struct VarList_
{
    char name[MAXNAMELEN];
    Type type;
    VarList tail;
};

struct FunDec_
{
    char name[MAXNAMELEN];
    Type type;
    VarList args;
};

typedef struct Symbol_ * Symbol;
typedef struct Symlist_* Symlist;
typedef struct Trie_* Trie;
typedef struct ICVariable_* ICVariable;
struct ICVariable_
{
	enum{VAR,TEMP,LAB,DNUM} kind;
	int cnt;
    int id;
};
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
extern int variablecnt,tempcnt,labelcnt;

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
    ICVariable icv;
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

//const int MAXVARIABLECNT=512;
//const int BITSIZE=64;

#define MAXVARIABLECNT  512
#define BITSIZE 64

struct bitset_
{
    ull d[MAXVARIABLECNT/BITSIZE];
};

typedef struct bitset_* bitset;
void setbit(bitset bs,int x);
void erasebit(bitset bs,int x);
void flipbit(bitset bs,int x);
bool testbit(bitset bs,int x);
void resetbit(bitset bs);
void unite(bitset x,bitset y);
void intersect(bitset x,bitset y);

//IR functions
void funcinit();
typedef struct InterCode_ *InterCode;
struct InterCode_
{
    InterCode prev,next;
    enum{LABEL,FUNC,ASSIGN,ADD,SUB,MUL,DIVI,ADDR,RVAL,LVAL,GOTO,IFGOTO,RET,DEC,ARG,CALL,PARAM,READ,WRITE} kind;
    union
    {
       struct{ICVariable op1,op2,op3; } ternary;
       struct{ICVariable op1,op2; } binary;
       struct{ICVariable op1; } unary;
       struct{ICVariable op1; int sz;} dec;
       struct{char funcname[MAXNAMELEN];} fundec;
       struct{ICVariable op1; char funcname[MAXNAMELEN];} funcall;
       struct{ICVariable op1,op2,op3; enum{LE_,GE_,GEQ_,LEQ_,EQ_,NEQ_} relop;} ig;
    }u;
    int id;
    bool isbasic;
} ;
ICVariable newvariable();
ICVariable newtemp();
ICVariable newlabel();
ICVariable direct_number(int d);
ICVariable find_icv(char *name);
void print_ICVariable(ICVariable v);
void print_intermediate_code(InterCode ic);
void print_intermediate_program(InterCode pt);
InterCode bind_code(InterCode code1,InterCode code2);
InterCode translate_Exp(ast node,ICVariable v);
InterCode translate_Cond(ast node,ICVariable label_true,ICVariable label_false);
InterCode translate_Stmt(ast node);
InterCode translate_Args(ast node);
InterCode translate_StmtList(ast node);
InterCode translate_VarDec(ast node,ICVariable v);
InterCode translate_Dec(ast node);
InterCode translate_DecList(ast node);
InterCode translate_Def(ast node);
InterCode translate_DefList(ast node);
InterCode translate_CompSt(ast node);
InterCode translate_ExtDecList(ast node);
InterCode translate_FunDec(ast node);
InterCode translate_VarList(ast node);
InterCode translate_ParamDec(ast node);
InterCode translate_ExtDef(ast node);
InterCode translate_ExtDefList(ast node);
InterCode translate_Program(ast node);

typedef struct MIPSOp_ *MIPSOp;
struct MIPSOp_
{
    enum{MOP_LABEL,MOP_REG,MOP_STA,MOP_DNUM} kind;
    union 
    {
        struct{char name[MAXNAMELEN]; int id;} label;
        struct{int id;}reg;
        struct{int reg_id; int offset;}sta;
        struct{int val;}dnum;
    }u; 
};

typedef struct MIPSCode_ *MIPSCode;
struct MIPSCode_
{
    MIPSCode prev,next;
    enum
    {
        MIPS_LABEL,MIPS_LI,MIPS_MOVE,MIPS_ADD0MIPS_SUB,MIPS_MUL,MIPS_DIV,MIPS_MFLO,MIPS_LA,
        MIPS_LW,MIPS_SW,MIPS_J,MIPS_JAL,MIPS_JR,MIPS_BEQ,MIPS_BNE,MIPS_BGT,MIPS_BLT,MIPS_BGE,MIPS_BLE
    }kind;
    union
    {
       struct{MIPSOp op1,op2,op3; } ternary;
       struct{MIPSOp op1,op2; } binary;
       struct{MIPSOp op1; } unary;
    }u;
};


//Object Code Functions
MIPSCode newmipscode();
MIPSCode bind_mipscode(MIPSCode code1,MIPSCode code2);
MIPSCode bind_mipscode3(MIPSCode code1,MIPSCode code2,MIPSCode code3);
MIPSCode bind_mipscode4(MIPSCode code1,MIPSCode code2,MIPSCode code3,MIPSCode code4);


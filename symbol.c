#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "defi.h"


int find_id(char x)
{
    if(x>='0'&&x<='9') return x-'0';
    else if(x>='A'&&x<='Z') return 10+x-'A';
    else return 36+x-'a';
}


Trie newnode()
{
    Trie t=(Trie)malloc(sizeof(struct Trie_));
    for(int i=0;i<60;i++) t->next[i]=NULL;
    t->sym=NULL;
    return t;
}

Trie func,var;
//trie root node for functions and variables respectively

void trieinit()
{
    func=newnode(); var=newnode(); 
}

//insert a new symbol into trie
//type=0,1,2 : function,variable, struct
bool trieinsert(Symbol sym)
{
    int type=sym->SymbolType;
    Trie s=(!type?func:var);
    char *x=sym->name;
    for(int i=0;x[i];i++)
    {
        int id=find_id(x[i]);
        if(s->next[id]) {s=s->next[id];}
        else
        {
            Trie t=newnode();
            s->next[id]=t;
            s=t;
        }
    }
    bool f=(s->sym==NULL); //return true if no replacement happened
    if(f) s->icv=newvariable();
    s->sym=sym;
    return f;
}
//check if some symbol exists in the trie
Symbol Function_Lookup(char *name)
{
    Trie s=func;
    char *x=name;
    for(int i=0;x[i];i++)
    {
        int id=find_id(x[i]);
        if(s->next[id]) {s=s->next[id];}
        else return NULL;
    }
    return s->sym;
}
Symbol Variable_Lookup(char *name)
{
    Trie s=var;
    char *x=name;
    for(int i=0;x[i];i++)
    {
        int id=find_id(x[i]);
        if(s->next[id]) {s=s->next[id];}
        else return NULL;
    }
    return s->sym;
}
//delete a symbol from trie
bool triedelete(Symbol sym)
{
    int type=sym->SymbolType;
    Trie s=(!type?func:var);
    char *x=sym->name;
    for(int i=0;x[i];i++)
    {
        int id=find_id(x[i]);
        s=s->next[id];
        if(s==NULL) return false;
    }
    if(s->sym==NULL) return false;
    s->sym=NULL;
    return true;
}
ICVariable find_icv(char *name)
{
    Trie s=var;
    char *x=name;
    for(int i=0;x[i];i++)
    {
        int id=find_id(x[i]);
        if(s->next[id]) {s=s->next[id];}
        else return NULL;
    }
    return s->icv;
}

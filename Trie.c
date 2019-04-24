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

struct trie
{
    struct trie* next[60];
    struct symbol *sym;
};


struct trie *newnode()
{
    struct trie* t=(struct trie*)malloc(sizeof(struct trie));
    for(int i=0;i<60;i++) t->next[i]=NULL;
    t->sym=NULL;
    return t;
}

struct trie *func,*var,*str;
//trie root node for functions,variables,and structs respectively

void TrieInit()
{
    func=newnode(); var=newnode(); str=newnode();
}

//insert a new symbol into trie
//type=0,1,2 : function,variable, struct
bool TrieInsert(struct symbol* sym,int type)
{
    struct trie* s=(!type?func:(type==1?var:str));
    char *x=sym->name;
    for(int i=0;x[i];i++)
    {
        int id=find_id(x[i]);
        if(s->next[id]) {s=s->next[id];}
        else
        {
            struct trie *t=newnode();
            s->next[id]=t;
            s=t;
        }
    }
    bool f=(s->sym==NULL); //return true if no replacement happened
    s->sym=sym;
    return f;
}
//check if some symbol exists in the trie
bool TrieFind(struct symbol* sym,int type)
{
    struct trie* s=(!type?func:(type==1?var:str));
    char *x=sym->name;
    for(int i=0;x[i];i++)
    {
        int id=find_id(x[i]);
        if(s->next[id]) {s=s->next[id];}
        else return false;
    }
    return (s->sym!=NULL);
}

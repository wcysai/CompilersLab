#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "defi.h"

bool SameType(Type p,Type q)
{
    if(p==NULL&&q==NULL) return true;
    if(p==NULL||q==NULL) return false;
    if(p->kind!=q->kind) return false;
    switch(p->kind)
    {
        case BASIC: return p->u.basic==q->u.basic;
        case ARRAY: return p->u.array.elem==q->u.array.elem;
        case STRUCTURE: return SameField(p->u.structure,q->u.structure);
    }
}

bool SameField(FieldList p,FieldList q)
{
    if(p==NULL&&q==NULL) return true;
    if(p==NULL||q==NULL) return false;
    return SameType(p->type,q->type)&&SameField(p->tail,q->tail);
}

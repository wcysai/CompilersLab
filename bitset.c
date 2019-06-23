#include <stdbool.h>
#include "defi.h"

void setbit(bitset bs,int x)
{
    bs->d[x/BITSIZE]|=(1ULL<<(x%BITSIZE));
}
void erasebit(bitset bs,int x)
{
   if(bs->d[x/BITSIZE]&(1ULL<<(x%BITSIZE))) bs->d[x/BITSIZE]^=(1ULL<<(x%BITSIZE));
}
void flipbit(bitset bs,int x)
{
    bs->d[x/BITSIZE]^=(1ULL<<(x%BITSIZE));
}
bool testbit(bitset bs,int x)
{
    return (bs->d[x]&(1ULL<<(x%BITSIZE)));
}
void resetbit(bitset bs)
{
    for(int i=0;i<MAXVARIABLECNT/BITSIZE;i++) bs->d[i]=0;
}
void unite(bitset x,bitset y)
{
    for(int i=0;i<MAXVARIABLECNT/BITSIZE;i++)
    {
        x->d[i]|=y->d[i];
    }
}
void intersect(bitset x,bitset y)
{
    for(int i=0;i<MAXVARIABLECNT/BITSIZE;i++)
    {
        x->d[i]&=y->d[i];
    }
}

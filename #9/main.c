#define USEAPI
#include "api.h"

int main ()
{
    int a=6,b=2,c=7,*p,i=0;
    listManager one;
    initList(&one);
    addListNode(&one,&a);
    addListNode(&one,&b);
    addListNode(&one,&c);
    setNextStart(&one,2);
    for(i=0;i<6;i++)
    {
        eachEntryNext(&one,(void**)&p,0);
        printf("%d.%d\n",i+1,*p);
    }
    return 0;
}
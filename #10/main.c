#include "api.h"
#define USEAPI

int main()
{
    int i=0,*intp;printf("MARKwtf");
    listManager one;
    printf("MARKinitbe");
    initHlist(&one,4);
    printf("MARKinit");
    for(i=0;i<10;i++)
    {
        intp = (int*)malloc(sizeof(int));
        *intp = i;
        addHlistNode(&one,i,(void*)intp);
    }
    popHlistNode(&one,7,(void*)intp);
    popHlistNode(&one,7,(void*)intp);
    //destroyHlist(&one);
}
#define USEAPI
#include "api.h"

int main()
{
    int i = 0, *intp;
    listManager one;
    initHlist(&one, 6);
    for (i = 0; i < 30; i++)
    {
        intp = (int *)malloc(sizeof(int));
        *intp = i;
        addHlistNode(&one, i, (void *)intp);
    }
    popHlistNode(&one, 7, (void **)&intp);
    popHlistNode(&one, 7, (void **)&intp);
    destroyHlist(&one);
    return 0;
}
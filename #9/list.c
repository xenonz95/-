#include "list.h"
#include "api.h"

/*
eachEntryNext(listManager* Man,void* p);
*/
static inline void _addListNode(listManager *Man, void *content)
{
    struct node *p = (struct node *)malloc(sizeof(struct node));

    p->carrier = content;
    if (!(Man->length))
    {
        Man->head = p;
        INIT_LIST_HEAD(&(p->list));
    }
    else
    {
        list_add_tail(&(p->list), &(Man->head->list));
    }
    ++(Man->length);
}

static inline void _popListNode(listManager *Man, void **content)
{
    struct node *p = container_of(Man->head->list.prev, struct node, list);
    *content = p->carrier;
    list_del(&(p->list));
    free(p);
    --(Man->length);
}

static inline void _setNextStart(listManager *Man, int number)
{
    struct list_head *np = &(Man->head->list);
    Man->avail = Man->length - number + 1;
    
    if (number && number <= Man->length)
    {
        while (--number)
        {
            np = np->next;
        }
        Man->p = np;
    }
    else
    {
        printf("out of the list, examine your number\n");
    }
}

static inline int _eachEntryNext(listManager *Man, void **p, int number)
{
    if (!number)
    {
        if (!(Man->p))
        {
            Man->avail = Man->length;
            Man->p = &(Man->head->list);
        }
    }
    else
    {
        _setNextStart(Man, number);
    }
    *p = container_of(Man->p, struct node, list)->carrier;
    Man->p = Man->p->next;
    if(!(--(Man->avail)))
        Man->p = NULL;
    return Man->avail;
}


void initList(listManager *Man)
{
    Man->length = 0;
    Man->head = NULL;
    Man->p = NULL;
}


void addListNode(listManager *Man, void *content)
{
    _addListNode(Man, content);
}


void popListNode(listManager *Man, void **content)
{
    _popListNode(Man, content);
}

static inline void _firstListNode(listManager *Man, void *content)
{
    content = Man->head->carrier;
}

void firstListNode(listManager *Man, void *content)
{
    _firstListNode(Man, content);
}

void destroyList(listManager *Man) ///Must free all carrier by yourself
{
    while (Man->length)
    {
        struct node *p = container_of(Man->head->list.prev, struct node, list);
        list_del(&(p->list));
        free(p);
        --(Man->length);
    }
}

int eachEntryNext(listManager *Man, void **p, int number)
{
    return _eachEntryNext(Man, p, number);
}


void setNextStart(listManager *Man, int number)
{
    _setNextStart(Man, number);
}
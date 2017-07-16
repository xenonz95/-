#include "list.h"
#include "api.h"

void initHlist(listManager *Man, int hash_number)
{
    int i = 0, j = 0;
    while ((hash_number - 1) >> ++i)
        ;
    Man->heads = (struct hlist_head *)malloc(sizeof(struct hlist_head) * i);
    Man->hash_number = i - 1;
    for (j = 0; j < i; j++)
    {
        INIT_HLIST_HEAD(Man->heads + j);
    }
}

void addHlistNode(listManager *Man, int key, void *content)
{
    struct node *p = (struct node *)malloc(sizeof(struct node));
    INIT_HLIST_NODE(&(p->list));
    p->carrier = content;
    p->key = key;
    hlist_add_head(&(p->list), &(Man->heads[key & (Man->hash_number)]));
    ++(Man->avail);
}

void popHlistNode(listManager *Man, int key, void **content)
{
    struct node *p1;
    struct hlist_head *p2;
    if (Man->avail)
    {
        if (hlist_empty(p2 = &(Man->heads[key & (Man->hash_number)])))
        {
            p1 = hlist_entry(p2->first, struct node, list);
            *content = p1->carrier;
            hlist_del(&(p1->list));
            free(p1);
            --(Man->avail);
        }
        else
        {
            printf("[debug] in this listManager, dont have the given key.");
            *content = NULL;
        }
    }
}

void findHlistNode(listManager *Man, int key, void **content)
{
    struct node *p1;
    struct hlist_head *p2;
    if (Man->avail)
    {
        if (hlist_empty(p2 = &(Man->heads[key & (Man->hash_number)])))
        {
            p1 = hlist_entry(p2->first, struct node, list);
            *content = p1->carrier;
        }
        else
        {
            printf("[debug] in this listManager, dont have the given key.");
            *content = NULL;
        }
    }
}

static void _defaultDestroier(struct node *a)
{
    free(a);
}

static inline void _destroyHlist(listManager *Man, void (*fun)(struct node *))
{
    int i = 0;
    struct node *p1;
    struct hlist_head *p2;
    while (Man->avail)
    {
        if (hlist_empty(p2 = &(Man->heads[i & (Man->hash_number)])))
        {
            p1 = hlist_entry(p2->first, struct node, list);
            fun(p1);
            --(Man->avail);
            i++;
        }
        else
        {
            i++;
            continue;
        }
    }
}

void destroyHlist(listManager *Man)
{
    _destroyHlist(Man, _defaultDestroier);
}

void destroyHlist_free(listManager *Man, void (*fun)(struct node *))
{
    _destroyHlist(Man, fun);
}
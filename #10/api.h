#include <stdio.h>
#include <stdlib.h>
#ifndef USEAPI
#include "list.h"
#else
struct list_head
{
	struct list_head *next, *prev;
};

struct hlist_head
{
	struct hlist_node *first;
};

struct hlist_node
{
	struct hlist_node *next, **pprev;
};
#endif

//增删查改 初始化 清除 遍历

struct node
{
	void *carrier;
	int key;
	struct hlist_node list;
};

struct _hlist
{
	int hash_number;
	int avail;
	struct hlist_head *heads;
};

typedef struct _hlist listManager;

void initHlist(listManager *Man, int hash_number);
void addHlistNode(listManager *Man, int key, void *content);
void popHlistNode(listManager *Man, int key, void **content);
void findHlistNode(listManager *Man, int key, void **content);
void destroyHlist(listManager *Man);
void destroyHlist_free(listManager *Man, void (*fun)(void *));
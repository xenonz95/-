#include "../head/BufferManager.h"
#include <iostream>

BufferManager::BufferManager(int length)
{
    if (length < 1)
        length = 1;
    int i = 0;
    while ((length - 1) >> ++i);
    space = new CircleBuffer(1 << i);
    pointer = new node;
    pointer->next = pointer;
}
int BufferManager::InString(const string &content)
{
    node *p = new node;
    p->next = pointer->next;
    pointer->next = p;
    p->length = content.length();
    pointer = p;
    for (int i = 0; i < pointer->length; i++)
    {
        space->InBuffer(content[i]);
    }
    return 0;
}
int BufferManager::OutString(string &content)
{
    node *p;
    content = string(pointer->next->next->length + 1, '\0');
    if (pointer->next == pointer)
        return -1;
    for (int i = 0; i < pointer->next->next->length; i++)
    {
        space->OutBuffer(content[i]);
    }
    p = pointer->next;
    pointer->next = pointer->next->next;
    delete p;
    return 0;
}
BufferManager::~BufferManager()
{
    delete space;
    node *p;
    while (pointer->next != pointer)
    {
        p = pointer->next;
        pointer->next = pointer->next->next;
        delete p;
    }
    delete pointer;
}
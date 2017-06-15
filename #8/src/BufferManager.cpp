#include "../head/BufferManager.h"
#include <iostream>

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
    // std::cout << pointer->length << endl;
    // std::cout << pointer->next->length << endl;
    // std::cout << pointer->next->next->length << endl;
    // std::cout << pointer->next->next->next->length << endl;
    // std::cout << pointer->next->next->next->next->length << endl << endl;
    return 0;
}
int BufferManager::OutString(string &content)
{
    // std::cout << pointer->length << endl;
    // std::cout << pointer->next->length << endl;
    // std::cout << pointer->next->next->length << endl;
    // std::cout << pointer->next->next->next->length << endl;
    // std::cout << pointer->next->next->next->next->length << endl << endl;
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
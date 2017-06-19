#include "../head/BufferManager.h"
#include <iostream>

BufferManager::BufferManager(int length)
{
    if (length < 1)
        length = 1;
    int i = 0;
    while ((length - 1) >> ++i);
    space = new CircleBuffer(1 << i);
}
int BufferManager::In(const char* content,int length)
{
    char a[4];
    int i=0;
    for(i=0;i<4;i++)
    {
        a[i]=length>>4*i;
    }
    for(i=0;i<4;i++)
    {
        space->InBuffer(a[i]);
    }
    for(i=0;i<length;i++)
    {
        space->InBuffer(((char*)content)[i]);
    }
    return 0;
}
char* BufferManager::Out(void)
{
    int i=0,length=0;
    char c;
    char* p;
    for(i=0;i<4;i++)
    {
        space->OutBuffer(c);
        length+=c<<4*i;
    }
    p=new char[length];
    for(i=0;i<length;i++)
    {
        space->OutBuffer(p[i]);
    }
    return p;
}
BufferManager::~BufferManager()
{
    delete space;
}
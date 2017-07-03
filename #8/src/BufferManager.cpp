#include "../head/BufferManager.h"
#include <iostream>

BufferManager::BufferManager(int length)
{
    if (length < 1)
        length = 1;
    int i = 0;
    while ((length - 1) >> ++i)
        ;
    space = new CircleBuffer(1 << i);
}
int BufferManager::In(const void *content, int length)
{
    char a[4];
    int i = 0, key = 0;
    for (i = 0; i < 4; i++)
    {
        a[i] = length >> 8 * i;
    }
    for (i = 0; i < 4; i++)
    {
        space->InBuffer(a[i]);
    }
    for (i = 0; i < length; i++)
    {
        if (!(key || space->fastIn(&((char *)content)[i], 8)||i+8>length))
            i += 7;
        else
        {
            //cout << i<< "do once\n";
            key = 1;
            space->InBuffer(((char *)content)[i]);
        }
    }
    return 0;
}
char *BufferManager::Out(void)
{
    int i = 0, length = 0, key = 0;
    char c;
    char *p;
    for (i = 0; i < 4; i++)
    {
        space->OutBuffer(c);
        length += c << 8 * i;
        cout << length<<endl;
    }
    p = new char[length];
    for (i = 0; i < length; i++)
    {
        if (!(key || space->fastOut(p + i, 8)||i+8>length))
            i += 7;
        else
        {
            cout << i <<"do once out\n";
            key = 1;
            space->OutBuffer(p[i]);
        }
    }
    return p;
}
BufferManager::~BufferManager()
{
    delete space;
}
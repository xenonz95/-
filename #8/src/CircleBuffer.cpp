#include "../head/CircleBuffer.h"

CircleBuffer::CircleBuffer(int length)
{
    CircleBuffer::length = length;
    data = new char[length];
    head = 0;
    end = head;
    mask = length-1;
};
int CircleBuffer::OutBuffer(char &c)
{
    if (Length() > 0)
    {
        c = data[head++&mask];
        return 0;
    }
    else
    {
        c = 0;
        return -1;
    }
}

int CircleBuffer::InBuffer(const char &c)
{
    if (Length() < length)
    {
        data[end++&mask] = c;
        return 0;
       
    }
    else return -1;
}

int CircleBuffer::Length(void)
{    return (end - head) &mask;}

int CircleBuffer::Available(void)
{    return length - CircleBuffer::Length();}

CircleBuffer::~CircleBuffer(void)
{    delete[] data;}
#include "../head/CircleBuffer.h"
CircleBuffer::CircleBuffer(int length)
{
    CircleBuffer::length = length;
    data = new char[length + 1];
    head = 0;
    end = head + 1;
};
int CircleBuffer::OutBuffer(char &c)
{
    if (CircleBuffer::Length() > 0)
    {
        c = data[++head];
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
    if (head != end)
    {
        data[end++] = c;
        return 0;
    }
    else return -1;
}

int CircleBuffer::Length(void)
{    return (end - head + length) % length-1;}

int CircleBuffer::Available(void)
{    return length - CircleBuffer::Length();}

CircleBuffer::~CircleBuffer(void)
{    delete[] data;}
#include "../head/CircleBuffer.h"
#include <string.h>
CircleBuffer::CircleBuffer(int length)
{
    CircleBuffer::length = length;
    data = new char[length];
    head = 0;
    end = head;
    mask = length - 1;
};

int CircleBuffer::fastOut(char *a, int len)
{
    if (len>Length() || (head& mask + len) > mask)
        return -1;
    else
    {
        memcpy(a, data + (const int)(head & mask), len);
        head += len;
        return 0;
    }
}

int CircleBuffer::OutBuffer(char &c)
{
    if (Length() > 0)
    {
        c = data[head++ & mask];
        return 0;
    }
    else
    {
        c = 0;
        return -1;
    }
}

int CircleBuffer::fastIn(char *a, int len)
{
    if (((end + len) & mask) < head || ((end + len) & mask) > length - 1)
        return -1;
    else
    {
        memcpy(data + (const int)(end & mask), a, len);
        end += len;
        return 0;
    }
}

int CircleBuffer::InBuffer(const char &c)
{
    if (Length() < length)
    {
        data[end++ & mask] = c;
        return 0;
    }
    else
        return -1;
}

int CircleBuffer::Length(void)
{
    return (end - head) & mask;
}

int CircleBuffer::Available(void)
{
    return length - CircleBuffer::Length();
}

CircleBuffer::~CircleBuffer(void)
{
    delete[] data;
}
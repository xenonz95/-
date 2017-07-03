#include "../head/fifo.h"

int work8::fifo_init(int Byte_count)
{
    one = new BufferManager(Byte_count);
    if (one == NULL)
        return -1;
    return 0;
}

int work8::fifo_in(string input_string)
{
    one->In(input_string.c_str(), input_string.length());
    return 0;
}

char *work8::fifo_out(void)
{
    return one->Out();
}

int work8::fifo_len(void)
{
    return one->length();
}

int work8::fifo_avail(void)
{
    return one->Available();
}
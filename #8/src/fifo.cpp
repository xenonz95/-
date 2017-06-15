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
    one->InString(input_string);
    return 0;
}

string work8::fifo_out(void)
{
    string content;
    one->OutString(content);
    return content;
}

int work8::fifo_len(void)
{
    return one->length();
}

int work8::fifo_avail(void)
{
    return one->Available();
}
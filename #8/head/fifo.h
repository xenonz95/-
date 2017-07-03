#include <iostream>
#include <string>
using namespace std;
#include "BufferManager.h"
class work8
{
public:
  int fifo_init(int Byte_count);
  int fifo_in(string input_string);
  char *fifo_out(void);
  int fifo_len(void);
  int fifo_avail(void);

private:
  BufferManager *one;
};
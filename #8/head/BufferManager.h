#include <string>
#include "../head/CircleBuffer.h"
using namespace std;

class BufferManager
{
  public:
    BufferManager(int length);
    int In(const char* content,int length);
    char* Out(void);
    inline int length()
    {   return space->Length();}
    inline int Available()
    {   return space->Available();}
    ~BufferManager();

  private:
    CircleBuffer *space;
};
#include <string>
#include "../head/CircleBuffer.h"
using namespace std;

class BufferManager
{
  public:
    BufferManager(int length);
    int InString(const string &content);
    int OutString(string &content);
    inline int length()
    {   return space->Length();}
    inline int Available()
    {   return space->Available();}
    ~BufferManager();

  private:
    struct node
    {
        int length;
        node *next;
    };
    node *pointer;
    CircleBuffer *space;
    int AddNode(int strLength);
    int RemoveNode(void);
};
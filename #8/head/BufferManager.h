#include <string>
#include "../head/CircleBuffer.h"
using namespace std;

class BufferManager
{
  public:
    BufferManager(int length)
    {
        space = new CircleBuffer(length);
        pointer = new node;
        pointer->next = pointer;
    }
    int InString(const string &content);
    int OutString(string &content);
    int length()
    {
        return space->Length();
    }
    int Available()
    {
        return space->Available();
    }
    ~BufferManager()
    {
        delete space;
        node *p;
        while (pointer->next != pointer)
        {
            p = pointer->next;
            pointer->next = pointer->next->next;
            delete p;
        }
        delete pointer;
    }

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
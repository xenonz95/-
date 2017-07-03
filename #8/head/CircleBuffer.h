class CircleBuffer
{
    friend class BufferManager;

  public:
    CircleBuffer(int length);
    int fastOut(char*a,int len);
    int OutBuffer(char &collecter);
    int fastIn(char *a,int len);
    int InBuffer(const char &collecter);
    int Length(void);
    int Available(void);
    ~CircleBuffer(); //maybe not use, but i think this class must have this function to prevent the memory leak
  private:
    int length;
    int head;
    int end;
    int mask;
    char *data;
};
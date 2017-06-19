class CircleBuffer
{
    friend class BufferManager;

  public:
    CircleBuffer(int length);
    int OutBuffer(char &collecter);
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
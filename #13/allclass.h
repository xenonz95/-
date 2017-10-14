#include "allhead.h"
using namespace std;

class coordinate;
class display;
class screen;
class shape;
class line;
class rectangle;
class word;
class manager;
class coursor;

class coordinate
{
  public:
    int x, y;

  public:
    coordinate(int x = 0, int y = 0);
    coordinate operator+(coordinate &A);
    coordinate operator-(coordinate &A);
    coordinate &operator=(std::initializer_list<int> il)
    {
        auto i = il.begin();
        x = *i;
        y = *(++i);
    }
    coordinate &debug();
    int index(int width);
};

class display
{
    int *display_mem_fb;
    int fd;
    int width, height;
    string address = "/dev/fb0";

  public:
    display()
    {
        openfd();
    }
    int openfd();
    void show(int *buffer);
    display &debug();
    void draw(int *buffer, coordinate pos, coordinate size);
    void draw(screen item);
};

class screen
{
    friend display;
    friend manager;
    int *buffer;
    coordinate pos;
    coordinate size;

  public:
    screen()
    {
        buffer = nullptr;
    }
    screen &setPos(coordinate pos);
    screen &setSize(coordinate size);
    screen &fullColor(int color);
    screen &debug();
};

class shape
{
protected:
    int color;
    coordinate box;
public:
    void setBox();
    virtual void draw(int *buffer);
    virtual shape &debug();
};
class point : public shape
{
  private:
    coordinate pointCo;

  public:
    point(coordinate _point, int _color = 0x00008888);
    virtual void draw(int *buffer);
    virtual point &debug();
};
class line : public shape
{
  private:
    coordinate point1;
    coordinate point2;

  public:
    line(coordinate point1, coordinate point2, int _color = 0x00888800);
    virtual void draw(int *buffer);
    virtual line &debug();
};
class rectangle : public shape
{
  private:
    coordinate pos;
    coordinate size;
  public:
    rectangle(coordinate pos, coordinate size, int _color = 0x00008888);
    virtual void draw(int *buffer);
    virtual rectangle &debug();
};
class word : public shape
{
  private:
    coordinate pos;
    char wordNum;
    int* font;
    int multiplySize;
    int lastWidth;
  public:
    word(coordinate point, int _color);
    void setFont(char word);
    virtual void draw(int *buffer);
    virtual word &debug();
};
class coursor:public shape
{
    coordinate pos;
public:
    coursor(coordinate point, int _color);
    virtual void draw(int *buffer);
    virtual coursor &debug();

};

class manager
{
    screen * target;
    vector<shape> randerLink;
    coursor mouse;
public:
    manager();
    void addShape(shape s);
    void print();
    manager& debug();
};

class input
{
  private:
    input_event event;
    int fileKey;
    char word;
    char direction; //just four bit means four key on keyboard
    char function;
    char confirm;

  public:
    input(string PATH);
    void update();
    char getWord();
    char getDirection(); //zreo means stop,1~8 start from right, anticolckwise.
    int getFun();
    bool getConfirm();
};

class funMode
{
  public:
    int function;
    int mode;
    funMode();
    void set(int a, int b);
    void used();
};
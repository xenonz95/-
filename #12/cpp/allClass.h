#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <math.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <sys/select.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <vector>
#include <initializer_list>

#include "_font.h"

using namespace std;

class item;

class coordinate
{
  public:
    char x, y;

  public:
    coordinate(char x = 0, char y = 0);
    coordinate operator+(coordinate &A);
    coordinate operator-(coordinate &A);
    coordinate &operator=(std::initializer_list<char> il)
    {
        auto i = il.begin();
        x = *i;
        y = *(++i);
    }
};
//------------------------------------------------------------------
class display
{
  private:
    int *fb;
    int fd;
    vector<vector<item>> layer;
    int *buffer;

  public:
    int height, width;

  public:
    display(string);
    void show();
    void addItem(item work, int layerNum);
    void clearLayer(int layerNum);
    ~display();
};
//------------------------------------------------------------------
class item
{
  protected:
    void full_xy_area(int x1, int y1, int x2, int y2, int color, int *buffer);
    void full_xy_line(int x1, int y1, int x2, int y2, int color, int *buffer);
    int *get_font(int no, int color);
    void print_a_word(int x, int y, int charno, int color, int *buffer);
    int color;

  private:
    int lastWidth = 0;
    int multiplySize = 3;

  public:
    virtual void draw(int *buffer);
};

class point : public item
{
  private:
    coordinate pointCo;

  public:
    point(coordinate _point);
    virtual void draw(int *buffer);
};
class line : public item
{
  private:
    coordinate point1;
    coordinate point2;

  public:
    line(coordinate point1, coordinate point2);
    virtual void draw(int *buffer);
};
class rectangle : public item
{
  private:
    coordinate point1;
    coordinate point2;

  public:
    rectangle(coordinate point1, coordinate point2);
    virtual void draw(int *buffer);
};
class word : public item
{
  private:
    coordinate point;
    char wordNum;

  public:
    word(coordinate point);
    void setFont(char word);
    virtual void draw(int *buffer);
};
//------------------------------------------------------------------
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
//------------------------------------------------------------------

class funMode
{
  public:
    int function;
    int mode;
    funMode();
    void set(int a, int b);
};
//------------------------------------------------------------------

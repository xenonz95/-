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



#include "allClass.h"

using namespace std;

class coordinate
{
  private:
    char x, y;

  public:
    coordinate(char x = 0, char y = 0);
    coordinate operator+(&A);
    coordinate operator-(&A);
}
//------------------------------------------------------------------
class display
{
  private:
    int *fb;
    int fd;
    vector<vector<item>> layer;
    int *buffer;
    int height, width;

  public:
    display(string);
    void show();
    void addItem(item work, int layerNum);
    ~display();
}
//------------------------------------------------------------------
class item
{
  protected:
    void full_xy_area(int x1, int y1, int x2, int y2, int color, int *buffer);
    void full_xy_line(int x1, int y1, int x2, int y2, int color, int *buffer);
    char out_code(int code);
    int *get_font(int no, int color);
    void print_a_word(int x, int y, int charno, int color, int *buffer);
    int codeToNo(int code);
    int color;

  private:
    int lastWidth = 0;
    int multiplySize = 3;

  public:
    virtual draw(int *buffer) = 0;
}

class point : public item
{
  private:
    coordinate point;

  public:
    point(coordinate point);
    virtual draw(int *buffer);
} class line : public item
{
  private:
    coordinate point1;
    coordinate point2;

  public:
    line(coordinate point1, coordinate point2);
    virtual draw(int *buffer);
} class rectangle : public item
{
  private:
    coordinate point1;
    coordinate point2;

  public:
    rectangle(coordinate point1, coordinate point2);
    virtual draw(int *buffer);
} class word : public item
{
  private:
    coordinate point;
    char wordNum;

  public:
    word(coordinate point);
    virtual draw(int *buffer);
}
//------------------------------------------------------------------
class input
{
  private:
    input_event event;
    int fileKey;
    char word;
    char direction; //zreo means stop,1~8 start from right, anticolckwise.
    int function;

  public:
    input(string PATH);
    void update();
    char getWord();
    char getDirection();
    int getFun();
}
//------------------------------------------------------------------

class funMode
{
  public:
    int function;
    int mode;
    funMode();
    void set(int a, int b);
}
//------------------------------------------------------------------

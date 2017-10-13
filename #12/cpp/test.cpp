#include "allClass.h"
#include <iostream>

#define PATH "/dev/input/event4"

int main()
{
    coordinate a(1,2),b(3,4),c(9,9);
    c = a+b;
    cout << c.x << ',' << c.y << endl;
    return 0;
}
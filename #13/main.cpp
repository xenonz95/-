//self defined
#include "allhead.h"
#include "allclass.h"
#include "_font.h"

int main()
{
    display a;
    coordinate b, c(3, 2), d,e(100,100),f(200,200);
    b.debug();
    (c + c).debug();
    d = {3, 5};
    d.debug();
    a.debug();
    screen s1;
    s1.setPos(e).setSize(f.debug());
    s1.fullColor(0xFF7777);
    cout <<"test\n";
    s1.debug();
    a.draw(s1);
    return 0;
}

#include "../head/fifo.h"
using namespace std;

int main()
{
    work8 test;
    test.fifo_init(100);
    test.fifo_in("Hello ");
    test.fifo_in("world ");
    test.fifo_in("\nand something");

    test.fifo_in("Hello ");
    test.fifo_in("world ");
    test.fifo_in("\nand something");

    cout << test.fifo_out();
    cout << test.fifo_out();
    cout << test.fifo_out() << endl << endl;
    cout << test.fifo_out();
    cout << test.fifo_out();
    cout << test.fifo_out() << endl << endl;
    // cout << "剩余空间：" << test.fifo_avail() << " "
    //      << "使用长度：" << test.fifo_len() << endl
    //      << endl;
    // cout << test.fifo_out();
    // cout << test.fifo_out();
    return 0;
}
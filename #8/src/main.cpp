#include "../head/fifo.h"
using namespace std;

int main()
{
    work8 test;
    test.fifo_init(100);
    for (int i = 0; i < 10; i++)
    {
        test.fifo_in("|1234567890|");
        test.fifo_in("|12345|");
        test.fifo_in("\nand something");
        cout << "剩余空间：" << test.fifo_avail() << " "
             << "使用长度：" << test.fifo_len() << endl
             << endl;
        cout << test.fifo_out();
        cout << "剩余空间：" << test.fifo_avail() << " "
             << "使用长度：" << test.fifo_len() << endl
             << endl;
        cout << test.fifo_out();
        cout << "剩余空间：" << test.fifo_avail() << " "
             << "使用长度：" << test.fifo_len() << endl
             << endl;
        cout << test.fifo_out();
        cout << "剩余空间：" << test.fifo_avail() << " "
             << "使用长度：" << test.fifo_len() << endl
             << endl;
    }

    // cout << test.fifo_out();
    // cout << test.fifo_out() << endl << endl;
    // cout << test.fifo_out();
    // cout << test.fifo_out();
    // cout << test.fifo_out() << endl << endl;
    // cout << "剩余空间：" << test.fifo_avail() << " "
    //      << "使用长度：" << test.fifo_len() << endl
    //      << endl;
    // cout << test.fifo_out();
    // cout << test.fifo_out();
    return 0;
}
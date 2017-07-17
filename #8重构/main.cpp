#include <iostream>
#include "fifo.h"
#include <string>
using namespace std;

int main() {
	fifo test;
	test.init(100);
	string str;
	char cp[20];
	for(int i=0;i<1000;i++)
	{
		test.in("hello world-one",16);
		test.out(cp,16);
		str = cp;
		cout << str;
	}

	return 0;
}

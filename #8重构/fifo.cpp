#include "fifo.h"
#include <string.h>

void fifo::init(int length) {
	int i = 0;
	for( i = 0 ; length >> i != 0 ; i ++);
	mask = length << (i - 1);
	space = new char[mask + 1];
	head = end = 0;
}

int fifo::in(const void* tp,int lens) {
	const char* p = (char*)tp;
	if(avail() < lens)
		return 1;
	int mark = end & mask;
	if(mark + lens < mask)
		memcpy(space + mark ,p ,lens);
	else {
		memcpy(space + mark ,p ,mask - mark);
		memcpy(space , p + mask-mark ,lens - mask + mark);
	}
	end += lens;
	return 0;
}

int fifo::out(void* tp,int lens) {
	char* p = (char*)tp;
	if(len() < lens) {
		return -1;
		lens = len();
	}
	int mark = head & mask;
	if (mark + lens < mask)
		memcpy(p ,space + mark ,lens);
	else {
		memcpy(p ,space + mark ,mask - mark);
		memcpy(p + mask-mark , space ,lens - mask + mark);
	}
	head += lens;
	return 0;
}

int fifo::len() {
	return (end - head)&mask;
}

int fifo::avail() {
	return mask + 1 - len();
}

fifo::~fifo() {
	delete space;
}

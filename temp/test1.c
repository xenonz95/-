#include "stdio.h"
#include "string.h"

int main(int argc, char ** argv)
{
	int mask = 0xFF;
	int end = 0x7FFFFFFF;
	int head = 0x7FFFFFFF -2;

	end += 3;

	printf("end: %d, head: %d, used bytes: %d \n", end, head, (end - head) & mask );
	return 0;
}

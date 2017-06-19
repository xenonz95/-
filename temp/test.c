#include "stdio.h"
#include "string.h"

int main(int argc, char ** argv)
{
	int length = 16;
	int mask = length -1;
	int end = 18;
	int head = 15;

	printf("end: %d, head: %d, used bytes: %d \n", end, head, (end - head) & mask );
	return 0;
}

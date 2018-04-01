#include <sys/file.h>
//for open syscall
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <stdlib.h>


#include <stdio.h>

int main(int argv, char const * argc[])
{
	int fd;
	int ret = 0;
	char buf[256] = {0,};
	int watting = 5;
	if(argv > 1)
	{
		sscanf(argc[1], "%d", &watting);
	}
	fd = open("file.txt", O_RDWR|O_APPEND );
	printf("%s\n", "===========before lock===========");
	ret = read(fd, buf, 256);
	if (ret != -1)
		printf("read :%s\n", buf);
	ret = write(fd, "1test text\n", 11);
	if (ret == -1)
		printf("%s\n", "error while write");


	ret = flock(fd, LOCK_EX);
	if(ret == -1)
	{
		printf("%s\n", "error while lock");
		exit(-1);
	}
	printf("%s\n", "===========after slock===========");
	ret = read(fd, buf, 256);
	if (ret != -1)
		printf("read :%s\n", buf);
	ret = write(fd, "2test text\n", 11);
	if (ret == -1)
		printf("%s\n", "error while write");
	printf("%s\n", "slocked");
	sleep(watting);
	printf("%s\n", "lock over");
	close(fd);
	return 0;
}
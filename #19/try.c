#include <sys/file.h>
//for open syscall
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//for exit()
#include <stdlib.h>

#include <stdio.h>

int main(int argv, char const * argc[])
{
	int fd;
	int ret = 0;
	char buf[256] = {0,};
	char name[64] = "file.txt";
	if(argv > 1)
	{
		sscanf("%s", name);
	}
	fd = open(name, O_RDWR|O_APPEND );
	if(fd == -1)
	{
		printf("%s\n", "error while open");
		exit(-1);
	}
	printf("%s\n", "===========open succeed===========");
	ret = read(fd, buf, 256);
	if (ret != -1)
		printf("read :%s\n", buf);
	ret = write(fd, "1test text\n", 11);
	if (ret == -1)
		printf("%s\n", "error while write");
	else
		printf("%s\n", "write ok");
	close(fd);
	fd = open(name, O_RDWR|O_APPEND );
	ret = read(fd, buf, 256);
	if (ret != -1)
		printf("=====\ncontent :%s\n", buf);

	close(fd);
	return 0;
}
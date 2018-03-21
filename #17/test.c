//for open syscall
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//for getopt function
#include <unistd.h>
//for exit()
#include <stdlib.h>
//for sscanf()
#include <stdio.h>
//for strlen()
#include <string.h>
//for mmap()
#include <sys/mman.h>
//for errno
#include <errno.h>

int main (int argc, char* const argv[])
{
	int line = 0;
	char mode[16] = {0,};
	char name[64] = "./file.txt";
	char data[256]= {0,};
	char buf[256] = {0,};
	int fd = 0;
	int cmd = 0;
	int flag = 0;
	char modec = 1;
	int i = 0;
	char *fdopen_mode = "w+";
	FILE *file = NULL;

	while( (cmd = getopt(argc, argv, "l:m:n:")) != -1)
	{
		printf("opt = %c, optarg = %s, optind = %d, argv[%d] = %s\n", cmd, optarg, optind, optind, argv[optind]);
		switch(cmd)
		{
			//which_line setting_mode 
			case 'l':
				sscanf(optarg, "%d", &line);
				break;
			case 'm':
				sscanf(optarg, "%s", mode);
				break;
			case 'n':
				sscanf(optarg, "%s", name);
				break;
			default :
				printf("cmd input error\n");
				//exit(-1);
				break;
		}
	}
	
	while (modec != '\0')
	{
		modec = mode[i++];
		switch(modec)
		{
			case 'c':flag |= O_CREAT; break;
			case 'r':flag |= O_RDONLY; break;
			case 'w':flag |= O_WRONLY; break;
			case 'a':flag |= O_APPEND; break;
			case 't':flag |= O_TRUNC; break;
			default:  break;
		}
	}
	
	fd = open(name, flag, S_IRUSR|S_IWUSR);
	printf("%s\n", "file content:");
	int ret = 0;
	while((ret = read(fd, buf, 256)) >= 0)
	{
		printf("%s\n", buf);
		if(ret == 0)
		{
			printf("%s\n", "read all over");
			break;
		}
	}
	printf("%s\n", "input data in next line");
	scanf("%s",data);
	ret = 0;
	int size = strlen(data);
	while(ret != size)
	{
		if((ret += write(fd, data, size)) < 0)
		{
			printf("%s\n", "error in 70L");
			break;
		}
	}
	close(fd);

	file = fopen("./fopen.txt", fdopen_mode);
	printf("%p\n", file);
	printf("%s\n", "===========here is fdopen===========");
	if(fseek(file, line, SEEK_SET) == 0)
	{
		printf("%s\n", "input fdopen data in next line");
		scanf("%s",data);
		fprintf(file, "%s\n", data);
	}

	char *map = NULL;
	printf("%s\n", "===========here is mmap===========");
	fd = open("./mmap.txt",
		O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR);
	lseek(fd, 128 - 1, SEEK_SET);
	write(fd , "e" , 1);
	close(fd);
	fd = open("./mmap.txt",	O_RDWR, S_IRUSR|S_IWUSR);
	int prot = PROT_READ|PROT_WRITE;
	int flags = MAP_SHARED;
	int offset = 0;
	map = mmap(NULL, 128, prot, flags, fd, offset);
	if(*(int*)map != -1)
	{
		for(i = 0; i < 128; i++)
		{
			map[i] = (char)i;
		}
	}
	else
	{
		printf("Error: failed to map fb buffer to memory.|%d\n", errno);
		close(fd);
		exit(-1);
	}
	printf("%s\n", "done!");
	close(fd);

	return 0;
}
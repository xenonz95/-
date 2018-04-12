#include <stdio.h>

//malloc
#include <stdlib.h>
//for open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/time.h>

#include <string.h>

typedef unsigned char bool;


unsigned int my_error = 0;//1 = open_fail 2 = get_stat_fail

bool is_dir(char* file_name)
{
	int ret = 0;
	struct stat statbuf;

	ret = stat(file_name, &statbuf);
	if(ret == -1)
	{
		printf("[error] while get stat\n");
		my_error = 2;
		return 0;
	}
	if(S_IFDIR & statbuf.st_mode)
		return 1;
	else 
		return 0;
}

int main(int argc, char* const argv[])
{
	//prepare something
	int block = 512*8;
	int ret = 0;
	char cpname[256] = "MVI_5612.mp4";
	char ptname[256] = "paste.mp4";
	long file_size = 0;
	int test_mode = 0;
	bool dir_mode = 0;

	while(1)
	{
		//copy_from paste_to 
		ret = getopt(argc, argv, "c:p:");
		if(ret == -1)
			break;
		switch(ret)
		{
			case 'c':
				sscanf(optarg, "%s", cpname);
				break;
			case 'p':
				sscanf(optarg, "%s", ptname);
				break;
			case '?' :
				printf("a wrong parameter");
				break;

			default:break;
		}

	}
	if(is_dir(cpname))
	{
		printf("enter dir mode\n");
		dir_mode = 1;
		printf("is dir\n");
	}
	else 
	{
		printf("is file\n");
	}
	
	if(my_error != 0)
		return -my_error;

	return 0;
}
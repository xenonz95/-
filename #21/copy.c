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
char* color[] = {"\033[0m", "\033[31m\033[1m", "\034[31m\033[1m", "\033[41m", "\033[44m"};//0 = normal 1 = red_font 2 = blue_font 3 = red_background 4 = blue_background
char* bar = "==================================================";
char* space = "                                                  ";
unsigned int my_error = 0;//1 = open_fail 2 = get_stat_fail 3 = parameter_error

void print_progress(double d1, double d2)
{
	static int old_p1 = 0, old_p2 = 0;
	int progress1 = d1*50;
	int progress2 = d2*50;
	
	if(old_p1 == progress1 && old_p2 == progress2)
		return;
	old_p1 = progress1;
	old_p2 = progress2;

	if(progress1 > progress2)
	{
		printf("\r[%s%s%s",color[4], bar+50-progress2, color[0]);
		printf("%s", bar+50-(progress1-progress2));
		printf("%s]",space+50-(50-progress1));
		printf("%3d%%",(int)(d2*100));
	}
	else{
		printf("\r[%s%s%s",color[4], bar+50-progress1, color[0]);
		printf("%s%s%s",color[4], space+50-(progress2-progress1), color[0]);
		printf("%s]",space+50-(50-progress2));
		printf("%3d%%",(int)(d2*100));

	}
	//printf("\t%s", cpname);
	fflush(stdout);

}


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

long get_size(char* file_name)
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
	return statbuf.st_size;
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
				my_error = 3;
				printf("[error]a wrong parameter\n");
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

	if(dir_mode == 0)
	{
		file_size = get_size(cpname);
		
	}
	int i = 0;
	int  j = 0;
	for(i = 0; i<100; i++)
	{
		for(j = 0;j<100; j++)
		{
			print_progress(j/100.0,i/100.0);
			usleep(10000);
		}
	}
	putchar('\n');

	return 0;
}
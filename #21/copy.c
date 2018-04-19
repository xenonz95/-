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

#define block 512*8
unsigned char buf[block];

typedef unsigned char bool;
char* color[] = {"\033[0m", "\033[31m\033[1m", "\034[31m\033[1m", "\033[41m", "\033[44m"};//0 = normal 1 = red_font 2 = blue_font 3 = red_background 4 = blue_background
char* bar = "==================================================";
char* space = "                                                  ";
unsigned int my_error = 0;//1 = open_fail 2 = get_stat_fail 3 = parameter_error
unsigned int my_warning = 0;// 1 = stack_empty 2 = queue_empty 

char* path_stack[65535] = {NULL,};
int stack_top = 0;
char* path_queue[65535] = {NULL,};
int queue_head = 0;
int queue_end = 0;


bool push(char* str)
{
	if(stack_top >= 65535)
		return -1;
	if(path_stack[stack_top] != NULL)
	{
		free(path_stack[stack_top]);
		path_stack[stack_top] = NULL;
	}
	path_stack[stack_top++] = strdup(str);
	return 0;
}
char* pop()
{
	if(stack_top < 0)
	{
		my_warning = 1;
		return NULL;
	}
	return path_stack[--stack_top];
}

bool enter(char* str)
{
	if(queue_head >= 65535)
	{
		return -1;
	}
	if(path_queue[queue_head] != NULL)
	{
		free(path_queue[queue_head]);
		path_queue[queue_head] = NULL;
	}
	path_queue[queue_head++] = str;
	return 0;
}

char* leave()
{
	if(queue_end == queue_head)
	{
		my_warning = 2;
		return NULL;
	}
	return path_queue[++queue_end];
}

int print_progress(double d1, double d2)
{
	//d1 is front, and d2 is background
	static int old_p1 = 0, old_p2 = 0;
	int progress1 = d1*50;
	int progress2 = d2*50;
	if(old_p1 == progress1 && old_p2 == progress2)
	{
		return 1;
	}
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
	fflush(stdout);
	return 0;
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

bool copy_bath(char* dest, char* src,int size)
{
	double dsize = size;
	int fdr = 0;
	int fdw = 0;
	int ret = 0;
	int cpd = 0;
	fdr = open(src, O_RDONLY);
	fdw = open(dest, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR|S_IWUSR | S_IRGRP | S_IROTH );
	while(1)
	{
		ret = read(fdr, buf, block);
		cpd += ret;
		write(fdw, buf, ret);
		if(print_progress(cpd/dsize, cpd/dsize) != 1)
			printf("  %-25.25s|", src);
		if(cpd == size)
			break;
	}
	close(fdr);
	close(fdw);
}

void get_dir_size(char* dest, char* src)
{
	long dir_size = 0;

	//首先深度遍历源文件夹，若目的目录不存在则创建目录，读取文件大小后将文件相对地址插入队列
	//深度优先遍历时使用栈存储工作目录相对地址
}

int main(int argc, char* const argv[])
{
	//prepare something
	int ret = 0;
	char cpname[256] = "MVI_5612.mp4";
	char ptname[256] = "paste.mp4";
	long file_size = 0;
	long dir_size = 0;
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
		copy_bath(ptname, cpname, file_size);
	}
	else{

	}






	
	putchar('\n');
	return 0;
}
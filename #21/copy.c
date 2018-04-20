#include <stdio.h>
//malloc
#include <stdlib.h>
//for open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//for readdir
#include <dirent.h>


#include <unistd.h>
#include <sys/time.h>

#include <string.h>

#define block 512*8 //512 is the best choice


#define DEBUG

#ifdef DEBUG
#define HERE {printf("%shere%s\n",color[1],color[0]);fflush(stdout);}
#endif

#ifndef DEBUG
#define HERE ;
#endif





unsigned char buf[block];

typedef unsigned char bool;
char* color[] = {"\033[0m", "\033[31m\033[1m", "\034[31m\033[1m", "\033[41m", "\033[44m"};//0 = normal 1 = red_font 2 = blue_font 3 = red_background 4 = blue_background
char* bar = "==================================================";
char* space = "                                                  ";
unsigned int my_error = 0;//1 = open_fail 2 = get_stat_fail 3 = parameter_error
unsigned int my_warning = 0;// 1 = stack_empty 2 = queue_empty 


struct work 
{
	char path[512];
	DIR *dp;
};

struct work path_stack[65535] = {{0,}};
int stack_top = 0;
char* path_queue[65535] = {NULL,};
int queue_head = 0;
int queue_end = 0;


bool push(struct work workin)
{
	if(stack_top >= 65535)
		return -1;
	path_stack[stack_top++] = workin;
	return 0;
}
struct work pop()
{
	if(stack_top <= 0)
	{
		my_warning = 1;
		return path_stack[0];
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
	if(queue_end >= queue_head)
	{
		my_warning = 2;
		return NULL;
	}
	return path_queue[queue_end++];
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
		printf("%s\n",file_name);
		printf("[error] while get stat\n");
		my_error = 2;
		exit(-1);
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
		printf("file name: %s\n",file_name);
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

long get_dir_size(char* dest, char* src)
{
	//准备工作
	//需要入栈的是opendir得到的目录流，根据文件信息进行判断
	long dir_size = 0;
	struct work now_work = {0,};
	char fullPaths[512]= {0,};
	char fullPathd[512]= {0,};
	char copy_path[512]= {0,};
	now_work.dp = opendir(src);
	struct dirent *fileinfo;
	mkdir(dest, 0777);
	
	//首先深度遍历源文件夹，若目的目录不存在则创建目录，读取文件大小后将文件相对地址插入队列
	while(my_warning != 1)
	{
		//循环,先readdir，如果是文件，则入队列，如果是目录，则入栈
		//如果readdir到了最后一个文件，则出栈，重复之前操作
		//如果栈空，则结束循环
		//目录树的根节点不入栈，path记录文件相对于源节点的路径名
		fileinfo = readdir(now_work.dp);
		if(fileinfo == NULL)
		{
			//pop并恢复状态
			now_work = pop();
			continue;
		}
		if(fileinfo->d_name[0] == '.')
		{
			continue;
		}
		strcpy(fullPaths, src);
		strcat(fullPaths, "/");
		strcat(fullPaths, now_work.path);
		strcat(fullPaths, "/");
		strcat(fullPaths, fileinfo->d_name);
		//printf("fullPaths:%s\n",fullPaths);
		if(is_dir(fullPaths))
		{
			push( now_work);
			if(now_work.path[0] != '\0')
				strcat(now_work.path, "/");
			strcat(now_work.path, fileinfo->d_name);
			strcpy(fullPathd, dest);
			strcat(fullPathd, "/");
			strcat(fullPathd, now_work.path);
			//printf("fullPathd:%s\n",fullPathd);
			mkdir(fullPathd, 0777);
			now_work.dp = opendir(fullPaths);
			
		}
		else{
			//printf("get a file\n");
			dir_size += get_size(fullPaths);
			strcpy(copy_path, now_work.path);
			if(now_work.path[0] != '\0')
				strcat(copy_path, "/");
			strcat(copy_path, fileinfo->d_name);
			enter(strdup(copy_path));
		}
	}
	//深度优先遍历时使用栈存储工作目录相对地址
	return dir_size;
}

int copy_dir_file(char* dest, char* src, long dir_size)
{
	char* relative_path = NULL;
	char fullPaths[512]= {0,};
	char fullPathd[512]= {0,};

	double dsize = 0;
	int size = 0;
	double ddir_size = dir_size;
	int fdr = 0;
	int fdw = 0;
	int ret = 0;
	long cpd = 0;
	long fullcpd = 0;

	while(my_warning != 2)
	{
		relative_path = leave();
		if(relative_path == NULL)
			break;
		strcpy(fullPaths, src);
		strcat(fullPaths, "/");
		strcat(fullPaths, relative_path);
		strcpy(fullPathd, dest);
		strcat(fullPathd, "/");
		strcat(fullPathd, relative_path);
		
		//printf("fullPathd:%s\n",fullPathd);

		size = get_size(fullPaths);
		dsize = size;
		cpd = 0;
		fdr = open(fullPaths, O_RDONLY);
		fdw = open(fullPathd, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR|S_IWUSR | S_IRGRP | S_IROTH );
		while(1)
		{
			ret = read(fdr, buf, block);
			cpd += ret;
			fullcpd += ret;
			write(fdw, buf, ret);
			if(print_progress(cpd/dsize, fullcpd/ddir_size) != 1)
			{
				if(strlen(fullPaths) > 25)
					printf("  %-25s", fullPaths + (strlen(fullPaths) - 25));
				else 
					printf("  %-25s", fullPaths);
			}
			if(cpd == size)
				break;
		}
		
		
		free(relative_path);
		close(fdr);
		close(fdw);

	}


}

int main(int argc, char* const argv[])
{
	//prepare something
	int ret = 0;
	char cpname[256] = "file";
	char ptname[256] = "paste";
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
		dir_size = get_dir_size(ptname, cpname);
		
		printf("dir_size:%ld\n",dir_size);
		copy_dir_file(ptname, cpname, dir_size);
	}

	putchar('\n');
	return 0;
}
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h> 


#define printfdir(fmt,arg...) printf("\033[1;34m"fmt"\033[0m",##arg)
#define printfexe(fmt,arg...) printf("\033[1;32m"fmt"\033[0m",##arg)



int maxL = 256;
int Larr[256] = {0,};

void printLarr()
{
	putchar('\t');
	printf("%s", "Larr: ");
	for(int i = 0; i<10 ; i++)
	{
		printf("%d ", Larr[i]);
	}
	return ;
}


void dir_oper(char const*path, int level, int end)
{
	if(level > maxL)
		return ;
	struct dirent *filename, *nextname;
	struct stat s_buf;
	DIR *dp = opendir(path);
	nextname = readdir(dp);
	while(nextname)
	{
		if(nextname->d_name[0] == '.')
		{
			nextname = readdir(dp);
		}
		else
			break;
	};
	/*readdir()必须循环调用，要读完整个目录的文件，readdir才会返回NULL 
	若未读完，就让他循环*/
	while(filename = nextname)
	{

		nextname = readdir(dp);
		while(nextname)
		{
			if(nextname->d_name[0] == '.')
			{
				nextname = readdir(dp);
			}
			else
				break;
		};

		if(filename->d_name[0] == '.')
			continue;
		/*判断一个文件是目录还是一个普通文件*/
		char file_path[200];
		bzero(file_path,200);
		strcat(file_path,path);
		strcat(file_path,"/");
		strcat(file_path,filename->d_name);
		

		

		/*获取文件信息，把信息放到s_buf中*/
		stat(file_path,&s_buf);

		for(int i = 0; i < level; i++ )
			{
				if(Larr[i] != 0)
					printf("    ");
				else
					printf("│   ");
			}

			if(nextname != NULL)
			{
				printf("├── ");
			}
			else
			{
				printf("└── ");
			}

		/*判断是否目录*/
		if(S_ISDIR(s_buf.st_mode))
		{
			printfdir("%s",filename->d_name);
			//printf("\t||%d %d",level,end);
			//printLarr();
			putchar('\n');
			if(nextname == NULL)
			{
				Larr[level] = 1;
				end = level + 1;
			}
			dir_oper(file_path, level + 1, end);
		}

		/*判断是否为普通文件*/
		if(S_ISREG(s_buf.st_mode))
		{
			if(S_IXOTH & s_buf.st_mode)
				printfexe("%s",filename->d_name);
			else
				printf("%s",filename->d_name);
			//printf("\t||%d %d",level,end);
			//printLarr();
			putchar('\n');
		}

	}
	Larr[level] = 0;
	return ;

}


int main(int argv, char const* argc[])
{
	char const* path = argc[1];
	struct stat s_buf;
	if(argv > 2)
		sscanf(argc[2], "%d", &maxL);
	/*获取文件信息，把信息放到s_buf中*/
	stat(path,&s_buf);

	/*判断输入的文件路径是否目录，若是目录，则往下执行，分析目录下的文件*/
	if(S_ISDIR(s_buf.st_mode))
	{
		printf("[%s]\n",path);
		dir_oper(path, 0, 0);
	}

	/*若输入的文件路径是普通文件，则打印并退出程序*/
	else if(S_ISREG(s_buf.st_mode))
	{
		printf("[%s] is a regular file\n",path);
		return 0;
	}
	return 0;
}
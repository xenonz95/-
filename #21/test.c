#include <stdio.h>

//malloc
#include <stdlib.h>
//for open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/time.h>

#include <math.h>
#include <string.h>


int testx[20] = {0};
int testy[20] = {0};

struct timeval tv1,tv2;
unsigned char* buffer;

#define Ghight 40
#define Gwidth 80

void tic()
{
	gettimeofday(&tv1,NULL);
}
double toc()
{
	gettimeofday(&tv2,NULL);
	double t1 = tv1.tv_sec + tv1.tv_usec/1000000.0;
	double t2 = tv2.tv_sec + tv2.tv_usec/1000000.0;
	return t2-t1;
}

int havepoint(int x, int y)
{
	x/=2;
	int yes = 0;
	int i;
	for(i = 0; i < 20 ; i++)
	{
		if(testx[i] == x)
		{
			if(testy[i] == y)
				return 1;
		}
	}
	return 0;
}

void showgraph()
{
	int i = 0,j = 0;
	for(i = Ghight-1;i >= 0; i--)
	{
		for(j = 0; j < Gwidth; j+=2)
		{
			//if(i == 0 || j == 0)
			//	printf("##");
			//else
			{
				if(havepoint(j,i))
				{	
					//putchar('*');
					//putchar(' ');
					putchar('(');
					putchar(')');
				}
				else{
					putchar(' ');
					putchar(' ');
				}
			}
		}
		putchar('\n');
	}
}

void test( int maxblock ,char* cpname, char* ptname)
{
	int ii = 0;
	int i = 0;
	double ret = 0;
	for(i = 4*8;i <= maxblock; i*=2,ii++)
	{
		int fdr,fdw;
		fdr = open(cpname, O_RDONLY);
		fdw = open(ptname, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR|S_IWUSR | S_IRGRP | S_IROTH );
		tic();
		while(1)
		{
			ret = read(fdr, buffer, i);
			if(ret == 0)
				break;
			write(fdw, buffer, i);
		}
		ret = log(toc())*8+20;
		testx[ii] = ii;
		testy[ii] = (int) ret;
		printf("block = %d over\n", i);
		close(fdr);
		close(fdw);
	}

}


char str[50] = "==================================================";
int main(int argc, char* const argv[])
{
	int block = 1024*1024*8;
	int ret = 0;
	char cpname[256] = "MVI_5612.mp4";
	char ptname[256] = "paste.mp4";
	long filesize = 0;
	int test_mode = 0;

	
	while( 1 )
	{
		//block ??? cpoy_from_[name] paste_to_[name] test_mode
		ret = getopt(argc, argv, "b:sc:p:t");
		if(ret == -1)
			break;
		switch(ret)
		{
			case 'b':
				sscanf(optarg, "%d", &block);
				break;
			case 's':
				break;
			case 'c':
				sscanf(optarg, "%s", cpname);
				break;
			case 'p':
				sscanf(optarg, "%s", ptname);
				break;
			case 't':
				test_mode = 1;
				break;
		
		default:break;
		}
	}
	buffer = (unsigned char*)malloc(block);
	int fdr = 0;
	int fdw = 0;

	test(block, cpname, ptname);


#if 0
	struct stat statbuff;
	fdr = open(cpname, O_RDONLY);
	fdw = open(ptname, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR|S_IWUSR | S_IRGRP | S_IROTH );
	if(stat(cpname, &statbuff) < 0)
	{
		printf("get the stat fail\n");
		return -1;
	}
	else
	{
		filesize = statbuff.st_size;
		printf("filesize = %ldKB\n", filesize/1024);
	}




	int i = 0;
	long filecp = 0;
	for(i = 0; i <= 50; filecp += ret )
	{
		ret = read(fdr, buffer, block);
		if(ret == -1)
		{
			printf("error!\n");
			break;
		}
		write(fdw, buffer, ret);
		
		if(filecp*50 / filesize - i > 0 )
		{
			i = (filecp*50 / filesize);
			printf("\r[%-50s]",str+50-i);
			printf("%3d%%",i*2);
			printf("\t%s", cpname);
			fflush(stdout);
		}
		
		if(ret == 0)
			break;
	}
	putchar('\n');

#endif
showgraph();
	free(buffer);
	close(fdr);
	close(fdw);

	
	return 0;
}
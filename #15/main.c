#include <stdio.h>  /*标准输入输出定义*/
#include <stdlib.h> /*标准函数库定义*/
#include <unistd.h> /*Unix 标准函数定义*/
#include <sys/types.h>  
#include <sys/stat.h>   
#include <fcntl.h>  /*文件控制定义*/
#include <termios.h>	/*PPSIX 终端控制定义*/
#include <errno.h>  /*错误号定义*/

#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>

#define LEN 256

int speed_arr[] = { B0, B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400};

int name_arr[] = {38400,  19200,  9600,  4800,  2400,  1200,  300, };

int main(int argc, char * const argv[])
{
	//init and read arg
	char name[20] = "/dev/ttyACM0";
	int wave = B38400;
	char temp = 0;
	char data[LEN] = {0,};
	char recv[LEN] = {0,};
	int fd = 0;
	struct termios newtio,oldtio;
	int arg = 0; 
	while( (arg = getopt(argc, argv, "d:n:w:t")) != -1 )
	{
		// name wave temp
		printf("opt = %c, optarg = %s, optind = %d, argv[%d] = %s\n"
			, arg, optarg, optind, optind, argv[optind]);
		switch( arg )
		{
			case 'd':
				strcpy(data, optarg); break;
			case 'n':
				strcpy(name, optarg); break;
			case 'w':
				sscanf(optarg, "%d", &wave); break;
			case 't':
				temp = 1; break;
		default : break;
		}
	}
	//open termio
	fd = open (name, O_RDWR | O_NOCTTY |O_NDELAY);
	if(fd == -1)
	{
		printf("open term fail\n");
		exit(-1);
	}
	//setting
	newtio.c_cflag |=  CLOCAL | CREAD;
	cfsetispeed(&newtio,wave);
	cfsetospeed(&newtio,wave);

	newtio.c_cflag &= ~CSIZE;
	newtio.c_cflag |= CS8;

	newtio.c_cflag |= PARENB;
	newtio.c_cflag |= PARODD;
	newtio.c_iflag |= (INPCK | ISTRIP);

	newtio.c_cflag &= ~CSTOPB;
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN]  = 1;

	tcflush(fd,TCIOFLUSH);
	tcsetattr(fd,TCSANOW,&newtio);
	//write and read

	int wn = 0;
	int rn = 0;

#if 0
	wn = write(fd, data, LEN);
	while(rn != wn)
	{
		if(rn > wn)
			printf("%s\n", "some thing wrong in main.c at 88L");
		rn += read(fd, recv+rn, LEN);
		break;
	}
	printf("data: %s\n", data);
	printf("wn = %d, rn = %d recv: %s\n", wn, rn, recv);
	//close
	close(fd);
#endif
	int exit = 0;
	struct timeval timeout={0,1000};
	fd_set fdR;
	wn = write(fd, data, LEN);
	char buf[LEN] = {0,};
	int wn2 = 0;
	while(!exit && rn != wn)
	{
		FD_ZERO(&fdR);
		FD_SET(fd,&fdR);
		FD_SET(0,&fdR);

		switch(select(fd + 1, &fdR,NULL, NULL, &timeout))
		{
			case -1: 
				printf("%s\n", "error at 109L");exit = 1;break; 
			case 0: 
				continue;break;
			default:
				if(FD_ISSET(0,&fdR))
					{

						memset(buf, 0, LEN);
						read( 0, buf, LEN);
						wn2 += write(fd, buf, LEN);
					}
				if(FD_ISSET(fd,&fdR))
					rn += read(fd, recv+rn, LEN);
				break;
		}
		printf("wn = %d, rn = %d recv: %s\n", wn, rn, recv);
		if(wn == rn || wn2 == rn)
		{
			rn = 0;
			wn2 = 0;

			memset(recv, 0, LEN);
		}
	}
	return 0;
}
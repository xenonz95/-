//
// Created by xenon on 2018/5/8.
//

//base
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
//network
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//other
#include <string.h>

int sendToIp(char *buf, uint16_t port, char *ip)
{
	ssize_t ret = 0;
	int sendFd;
	if ((sendFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		exit(-1);
	}
	struct sockaddr_in sendSock;
	memset(&sendSock, 0, sizeof(sendSock));
	sendSock.sin_family = AF_INET;
	sendSock.sin_port = htons(port);
	sendSock.sin_addr.s_addr = inet_addr(ip);
	ret = connect(sendFd, (struct sockaddr *) &sendSock, sizeof(sendSock));
	if (ret == 0)
	{
		printf("connect is ok\n");
	}
	else if(ret == -1)
	{
		perror("[connect fail]");
	}
	ret = send(sendFd, buf, strlen(buf),0);

	printf("send %zi byte\n", ret);
	shutdown(sendFd, SHUT_RDWR);//比起用close()要更温和,会发送完剩下的缓冲区的内容
	close(sendFd);
}

int main()
{
	char buf[256] = "data is there\n";
	sendToIp(buf, 8888, "127.0.0.1");
	return 0;
}


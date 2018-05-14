//
// Created by xenon on 2018/5/7.
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

int sendToIp(char *buf, char *ip)
{
	ssize_t ret = 0;
	int sendFd;
	if ((sendFd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		exit(-1);
	}
	struct sockaddr_in sendSock;
	memset(&sendSock, 0, sizeof(sendSock));
	sendSock.sin_family = AF_INET;
	sendSock.sin_port = htons(6666);
	sendSock.sin_addr.s_addr = inet_addr(ip);
	ret = sendto(sendFd, buf, strlen(buf), 0, (struct sockaddr *) &sendSock,
	             sizeof
	             (sendSock));

	printf("send %zi byte", ret);
}


int main()
{
	char buf[256] = "information\n";
	sendToIp(buf, "192.168.10.255");

	return 0;
}
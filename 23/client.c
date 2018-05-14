//
// Created by xenon on 2018/5/3.
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

#define ERR_EXIT(m) \
        do \
        { \
                perror(m); \
                exit(EXIT_FAILURE); \
        } while(0);




int main(void)
{
	int sock;
	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
		ERR_EXIT("socket");

	struct sockaddr_in in;
	bzero(&in, sizeof(in));
	in.sin_family = AF_INET;
	in.sin_port = htons(4242);
	in.sin_addr.s_addr = inet_addr("127.0.0.1");

	int ret;
	char sendbuf[1024] = {0};
	char recvbuf[1024] = {0};
	while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
	{

		sendto(sock, sendbuf, strlen(sendbuf), 0, (struct sockaddr *) &in,
		       sizeof(in));

		ret = (int) recvfrom(sock, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
		if (ret == -1)
		{
			if (errno == EINTR)
				continue;
			ERR_EXIT("recvfrom");
		}

		fputs(recvbuf, stdout);
		memset(sendbuf, 0, sizeof(sendbuf));
		memset(recvbuf, 0, sizeof(recvbuf));
	}
	return 0;
}
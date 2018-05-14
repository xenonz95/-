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
//other
#include <string.h>

#define ERR_EXIT(m) \
    do { \
        perror(m); \
        exit(EXIT_FAILURE); \
    } while (0);

#define PORT 4242

int main(int argc, const char *argv[])
{
	int sock;
	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
		ERR_EXIT("socket error");

	struct sockaddr_in in;
	memset(&in, 0, sizeof(in));

	in.sin_family = AF_INET;
	in.sin_port = htons(PORT);
	in.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock, (struct sockaddr *) &in, sizeof(in)) < 0)
		ERR_EXIT("bind error");

	char buf[1024] = {0};
	struct sockaddr_in in2;
	socklen_t socklen;
	int n;//size of data length

	while (1)
	{
		socklen = sizeof(in2);
		memset(buf, 0, sizeof(buf));
		n = (int) recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *) &in2,
		                   &socklen);
		if (n == -1)
		{
			if (errno == EINTR)
			{
				continue;
			}
			ERR_EXIT("receive error");
		}
		else if (n > 0)
		{
			fputs(buf, stdout);
			fflush(stdout);
			sendto(sock, buf, (size_t) n, 0, (struct sockaddr *) &in2, socklen);
		}
	}

}
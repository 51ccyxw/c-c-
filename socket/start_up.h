#ifndef _START_UP_H
#define _START_UP_H

#include"utili.h"
#define LISTEN_QUEUE_SIZE 5

int start_up(const char* ip,short port,int mode)
{
	int sockfd;
	if(mode == tcp)
		sockfd = socket(AF_INET,SOCK_STREAM,0);
	else if(mode == udp)
		sockfd = socket(AF_INET,SOCK_DGRAM,0);
	else
	{
		perror("mode!");
		return -1;
	}

	struct sockaddr_in addrser;
	addrser.sin_family = AF_INET;
	addrser.sin_port = htons(port);
	addrser.sin_addr.s_addr = inet_addr(ip);

	int yes = 1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));

	int ret = bind(sockfd,(struct sockaddr*)&addrser,addrlen);
	if(ret == -1)
	{
		perror("bind.");
		return -1;
	}

	if(mode == tcp)
	{
		ret = listen(sockfd,LISTEN_QUEUE_SIZE);
		if(ret == -1)
		{
			perror("listen.");
			return -1;
		}
	}
	return sockfd;
}

#endif

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<poll.h>

#define MAX_CLIENT_COUNT 5
#define CLIENT_QUEUE_SIZE 5
#define BUFER_SIZE 256

int main(int argc,char **argv)
{
	int sockser = socket(AF_INET,SOCK_STREAM,0);
	if(sockser == -1)
	{
		perror("socket");
		exit(1);
	}

	socklen_t addrlen  = sizeof(struct sockaddr);

	struct sockaddr_in addrser,addrcli;
	addrser.sin_family = AF_INET;
	addrser.sin_port = htons(atoi(argv[2]));
	addrser.sin_addr.s_addr = inet_addr(argv[1]);

	int yes = 1;
	setsockopt(sockser,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));

	int ret = bind(sockser,(struct sockaddr*)&addrser,addrlen);
	if(ret == -1)
	{
		perror("bind");
		exit(1);
	}
	ret = listen(sockser,CLIENT_QUEUE_SIZE);
	if(ret == -1)
	{
		perror("listen");
		exit(1);
	}

	struct pollfd client_fd[MAX_CLIENT_COUNT+1]; //1是服务器
	int i,num = 1,sockcli;
	client_fd[0].fd = sockser;
	client_fd[0].events = POLLIN;
	char buf[BUFER_SIZE];

	for(i = 1;i <= MAX_CLIENT_COUNT;++i)
	{
		client_fd[i].fd = 0;
	}

	while(1)
	{
		int ret = poll(client_fd,num,3000);
		if(ret == -1)
		{
			perror("poll");
			continue;
		}
		if(ret == 0)
		{
			printf("TIME OUT\n");
			continue;
		}
		else
		{
			if((client_fd[0].fd == sockser) && (client_fd[0].revents & POLLIN))
			{
				printf("server wait client connect!\n");
				sockcli = accept(client_fd[0].fd,(struct sockaddr*)&addrcli,&addrlen);
				if(sockcli == -1)
				{
					perror("accept");
					continue;
				}
				for(i = 1;i <= MAX_CLIENT_COUNT;++i)
				{
					if(client_fd[i].fd == 0)
					{
						client_fd[i].events = POLLIN;
						client_fd[i].fd = sockcli;
						++num;
						break;
					}
				}
				if(i >= MAX_CLIENT_COUNT+1)
				{
					printf("Server Over Load!\n");
				}
				else
				{
					printf("New client Come\n");
				}
			}
			else
			{
				for(i = 1;i <= MAX_CLIENT_COUNT;++i)
				{
					if((client_fd[i].fd != 0) && (client_fd[i].revents & POLLIN))
					{
						int recvbyte = recv(client_fd[i].fd,buf,256,0);
						if(recvbyte < 0)
						{
							printf("recv data error!\n");
							continue;
						}
						else if(recvbyte == 0)
						{
							printf("one client leave!\n");
							continue;
						}
						else
						{
							printf("recv from client:%s\n",buf);
						}
						int sendbyte = send(client_fd[i].fd,buf,strlen(buf)+1,0);
						if(sendbyte < 0)
						{
							perror("send");
							continue;
						}
					}
				}
			}
		}
	}
	close(sockser);
	return 0;
}

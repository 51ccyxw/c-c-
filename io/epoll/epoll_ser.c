#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/epoll.h>

#define MAX_CLIENT_NUM 5
#define CLIENT_QUEUE_SIZE 5
#define BUFER_SIZE 256

void event_add(int ev_t_fd,uint32_t event,int fd)
{
	struct epoll_event ev;
	ev.events = event;
	ev.data.fd = fd;
	epoll_ctl(ev_t_fd,EPOLL_CTL_ADD,fd,&ev);
}

int main(int argc,char** argv)
{
	int sockser = socket(AF_INET,SOCK_STREAM,0);
	if(sockser == -1)
	{
		perror("socket");
		exit(1);
	}

	socklen_t addrlen = sizeof(struct sockaddr);
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
	
	char buf[BUFER_SIZE];
	int epoll_fd = epoll_create(MAX_CLIENT_NUM+1);
	struct epoll_event ev;
	struct epoll_event revent[MAX_CLIENT_NUM];
	event_add(epoll_fd,EPOLLIN,sockser);
	int i,sockcli;

	while(1)
	{
		ret = epoll_wait(epoll_fd,revent,MAX_CLIENT_NUM,-1);
		if(ret == -1)
		{
			perror("epoll_wait");
			continue;
		}
		if(ret == 0)
		{
			printf("Time Out\n");
			continue;
		}
		else
		{
			for(i = 0;i < ret;++i)
			{
				if(revent[i].data.fd == sockser && (revent[i].events & EPOLLIN))
				{
					sockcli = accept(revent[i].data.fd,(struct sockaddr*)&addrcli,&addrlen);
					if(sockcli == -1)
					{
						perror("accept");
						break;
					}
					else
					{
						event_add(epoll_fd,EPOLLIN,sockcli);
						printf("New Client Come!\n");
					}
				}
				else if(revent[i].events & EPOLLIN)
				{
					int recvbyte = recv(revent[i].data.fd,buf,256,0);
					if(recvbyte < 0)
					{
						perror("recv");
						continue;
					}
					else if(recvbyte == 0)
					{
						printf("One Client leave!\n");
						continue;
					}
					else
					{
						printf("recv from client%d:%s\n",i,buf);
					}
					int sendbyte = send(revent[i].data.fd,buf,strlen(buf)+1,0);
					if(sendbyte < 0)
					{
						perror("send");
						break;
					}
				}
			}
		}
	}

	close(sockser);
	return 0;
}

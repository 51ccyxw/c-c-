#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<sys/time.h>
#include<netinet/in.h>

#define LISTEN_QUEUE_SIZE 5
#define BUFER_SIZE 256
#define MAX_CLIENT_COUNT 5

int main(int argc,char **argv)
{
	int sockser = socket(AF_INET,SOCK_STREAM,0);
	if(sockser == -1)
	{
		perror("socket:");
		exit(1);
	}
	struct sockaddr_in addrser,addrcli;
	addrser.sin_family = AF_INET;
	addrser.sin_port = htons(atoi(argv[2]));
	addrser.sin_addr.s_addr = inet_addr(argv[1]);

	socklen_t addrlen = sizeof(struct sockaddr);

	int yes = 1;
	setsockopt(sockser,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));

	int ret = bind(sockser,(struct sockaddr*)&addrser,addrlen);
	if(ret == -1)
	{
		perror("bind:");
		exit(1);
	}
	ret = listen(sockser,LISTEN_QUEUE_SIZE);
	if(ret == -1)
	{
		perror("listen:");
		exit(1);
	}

	char buf[BUFER_SIZE] = {0};
	int client_fd[MAX_CLIENT_COUNT] = {0};
	int client_count = 0;
	int i,sockcli;
	fd_set set;

	int max_sock = sockser;

	while(1)
	{
		FD_ZERO(&set);
		FD_SET(sockser,&set);
		for(i = 0;i < MAX_CLIENT_COUNT;++i)
		{
			if(client_fd[i] != 0)
			{
				FD_SET(client_fd[i],&set);
			}
		}

		struct timeval tv;
		tv.tv_sec = 3;
		tv.tv_usec = 0;

		ret = select(max_sock+1,&set,NULL,NULL,&tv);
		if(ret == -1)
		{
			perror("select:");
			continue;
		}
		if(ret == 0)
		{
			printf("TIME OUT\n");
			continue;
		}
		else
		{
			if(FD_ISSET(sockser,&set))
			{
				printf("server wait client connect\n");
				sockcli = accept(sockser,(struct sockaddr*)&addrcli,&addrlen);
				if(sockcli == -1)
				{
					perror("accept:");
					continue;
				}
				if(sockcli > max_sock)
				{
					max_sock = sockcli;
				}
				++client_count;
				for(i = 0;i < MAX_CLIENT_COUNT;++i)
				{
					if(client_fd[i] == 0)
					{
						client_fd[i] = sockcli;
						break;
					}
				}
				if(i >= MAX_CLIENT_COUNT)
				{
					printf("Server Over Load!!!!\n");
				}
				else
				{
					printf("New client come\n");
				}
			}
			else
			{
				for(i = 0;i <MAX_CLIENT_COUNT;++i)
				{
					if(client_fd[i] != 0 && FD_ISSET(client_fd[i],&set))
					{
						int recvbyte = recv(client_fd[i],buf,256,0);
						if(recvbyte <=  0)
						{
							printf("recv data error!\n");
						}
						int sendbyte = send(client_fd[i],buf,strlen(buf)+1,0);
						if(sendbyte <= 0)
						{
							printf("send data error!\n");
						}
						break;
					}
				}
			}
		}

	}
	
	close(sockser);
	return 0;
}

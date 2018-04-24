#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>

#define BUFER_SIZE 256

int main(int argc,char **argv)
{
	int sockcli = socket(AF_INET,SOCK_STREAM,0);
	if(sockcli == -1)
	{
		perror("socket");
		exit(1);
	}

	socklen_t addrlen = sizeof(struct sockaddr);

	struct sockaddr_in addrser;
	addrser.sin_family = AF_INET;
	addrser.sin_port = htons(atoi(argv[2]));
	addrser.sin_addr.s_addr = inet_addr(argv[1]);
	
	int ret = connect(sockcli,(struct sockaddr*)&addrser,addrlen);
	if(ret == -1)
	{
		perror("connect");
		exit(1);
	}
	else
	{
		printf("client connect server success!\n");
	}

	char bufsend[BUFER_SIZE] = {0};
	char bufrecv[BUFER_SIZE] = {0};

	while(1)
	{
		printf("cli:");
		scanf("%s",bufsend);
		send(sockcli,bufsend,strlen(bufsend)+1,0);
		recv(sockcli,bufrecv,256,0);
		printf("recv from ser:%s\n",bufrecv);
	}

	close(sockcli);
	return 0;
}

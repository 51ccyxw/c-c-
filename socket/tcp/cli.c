#include"../utili.h"

#define BUFSIZE 256

int main(int argc,char **argv)
{
	int sockcli = socket(AF_INET,SOCK_STREAM,0);
	if(sockcli == -1)
	{
		perror("socket.");
		exit(1);
	}

	struct sockaddr_in addrser;
	addrser.sin_family = AF_INET;
	addrser.sin_port = htons(atoi(argv[2]));
	addrser.sin_addr.s_addr = inet_addr(argv[1]);
	
	int ret = connect(sockcli,(struct sockaddr*)&addrser,addrlen);
	if(ret == -1)
	{
		perror("connect!");
		exit(1);
	}
	
	char recvbuf[BUFSIZE];
	char sendbuf[BUFSIZE];

	while(1)
	{
		recv(sockcli,recvbuf,256,0);
		printf("ser:%s\n",recvbuf);
		printf("cli:");
		scanf("%s",sendbuf);
		send(sockcli,sendbuf,strlen(sendbuf)+1,0);
	}

	close(sockcli);
	return 0;
}

#include"../utili.h"

#define BUF_SIZE 256

int main(int argc,char **argv)
{
	int sockcli = socket(AF_INET,SOCK_DGRAM,0);
	if(sockcli == -1)
	{
		perror("socket.");
		exit(1);
	}

	struct sockaddr_in addrser;
	addrser.sin_family = AF_INET;
	addrser.sin_port = htons(atoi(argv[2]));
	addrser.sin_addr.s_addr = inet_addr(argv[1]);

	char sendbuf[BUF_SIZE];
	char recvbuf[BUF_SIZE];

	while(1)
	{
		printf("cli:");
		scanf("%s",sendbuf);
		sendto(sockcli,sendbuf,strlen(sendbuf)+1,0,(struct sockaddr*)&addrser,addrlen);
		recvfrom(sockcli,recvbuf,256,0,(struct sockaddr*)&addrser,&addrlen);
		printf("ser:%s\n",recvbuf);
	}

	close(sockcli);
	return 0;
}


#include"../start_up.h"

#define BUF_SIZE 256

int main(int argc,char **argv)
{
	int sockser = start_up(argv[1],atoi(argv[2]),tcp);

	struct sockaddr_in addrcli;
	int sockcli = accept(sockser,(struct sockaddr*)&addrcli,&addrlen);
	if(sockcli == -1)
	{
		perror("accept.");
		exit(1);
	}
	else
	{
		printf("server accept an client success!\n");
	}

	char recvbuf[BUF_SIZE];
	char sendbuf[BUF_SIZE];

	while(1)
	{
		printf("ser:");
		scanf("%s",sendbuf);
		send(sockcli,sendbuf,strlen(sendbuf)+1,0);
		recv(sockcli,recvbuf,256,0);
		printf("cli:%s\n",recvbuf);
	}
	close(sockser);
	return 0;
}

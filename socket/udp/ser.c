#include"../start_up.h"

#define BUF_SIZE 256

int main(int argc,char **argv)
{
	int sockser = start_up(argv[1],atoi(argv[2]),udp);

	char recvbuf[BUF_SIZE];
	char sendbuf[BUF_SIZE];

	struct sockaddr_in addrcli;

	while(1)
	{
		recvfrom(sockser,recvbuf,256,0,(struct sockaddr*)&addrcli,&addrlen);
		printf("from (%s,%d)\nmessage:%s\n",inet_ntoa(addrcli.sin_addr),addrcli.sin_port,recvbuf);
		sendto(sockser,recvbuf,strlen(recvbuf)+1,0,(struct sockaddr*)&addrcli,addrlen);
	}

	close(sockser);
	return 0;
}

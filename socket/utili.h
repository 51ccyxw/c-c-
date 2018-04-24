#ifndef _UTILI_H
#define _UTILI_H

#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>

socklen_t addrlen = sizeof(struct sockaddr);
enum{tcp,udp};

#endif

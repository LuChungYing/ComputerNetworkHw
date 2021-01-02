#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
int main(int argc,char* argv[]){
	//head=0;tail=0;
	char pack[10];
	struct sockaddr_in Server,client;
	int socketfd;
	int length=sizeof(struct sockaddr);
	//memset(portlist,0,sizeof(portlist));
	int port=atoi(argv[1]);
	memset(&Server, 0, sizeof(Server));
	memset(&client, 0, sizeof(client));
	if((socketfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
		printf("socket");
	Server.sin_family = AF_INET;
	Server.sin_port = port;
	Server.sin_addr.s_addr = INADDR_ANY;
	if(bind(socketfd, (struct sockaddr *) &Server, sizeof(Server)) == -1)
		printf("bind\n");
	struct timeval nNewTimeout={0};
	
	nNewTimeout.tv_usec = 10;
	setsockopt(socketfd,SOL_SOCKET,SO_RCVTIMEO, (char *)&nNewTimeout,sizeof(struct timeval));
	if (recvfrom(socketfd, pack, sizeof(pack), 0, (struct sockaddr*)&client , &length) <0)
                        perror ("could not read datagram!!");
	printf("%s",pack);
} 

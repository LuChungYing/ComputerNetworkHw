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
  struct sockaddr_in server;
	static int socketfd;
	int n;
	int length = sizeof(struct sockaddr_in);
	int nbytes;
	//Req req;req.request=0;
	int cli_seq,cli_ack;
	/*for(int i=4; argc-i>0; i++){
		int a = atoi(argv[i]);
		int two=1;
		for(int x=0;x<a;x++) two*=2;
		req.request = req.request|(two);
	}req.request >>= 1;*/
	char pack[]="123";
	memset((char*)&server,0,sizeof(server));
		server.sin_addr.s_addr =inet_addr(argv[1]);
		server.sin_family = AF_INET;
    server.sin_port = atoi(argv[2]);
    if((socketfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
		printf("socket");

		
	printf("=====Start the three-way hand shake=====\n");
		if (sendto(socketfd, pack, sizeof(pack), 0, (struct sockaddr*)&server, length) < 0) {
		        perror("Could not send datagram!!\n");
		        printf("1:%d\n",errno);
    }
}

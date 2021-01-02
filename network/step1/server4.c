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


#define BUFFERSIZE 1024 //32kb
#define MAXNAME 1024
typedef struct TCP_header{
	uint16_t source_port;
	uint16_t dest_port;
	uint32_t seq;
	uint32_t ACK;
	uint16_t flag;
	uint16_t recwd;
	uint16_t checksum;
	uint16_t URG;
}TCP_header;                    //5*4 = 20 bytes

typedef struct segment{
	TCP_header header;
	char data[1004];

}segmemt;


int main(int argc,char* argv[]){
	struct sockaddr_in server;
	int socketfd, n;
	socklen_t length;
	int nbytes;
	char recbuf[BUFFERSIZE];
	char sendbuf[BUFFERSIZE];
	int port=atoi(argv[1]);
	memset((char*)&server,0,sizeof(server));

struct sockaddr_in client = calloc(1, sizeof(client));

	memset(&server, 0, sizeof(server));
	//memset(&client, 0, sizeof(client));

	if((socketfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
		printf("socket");

	server.sin_family = AF_INET;
	server.sin_port = port;
	server.sin_addr.s_addr = INADDR_ANY;
	if(bind(socketfd, (struct sockaddr *) &server, sizeof(server)) == -1)
		printf("bind");

	/*FILE* fptr = fopen("1.mp4","rb");
	char data[1004];
	memset(data,0,sizeof(data));
  int num=0;
	while(!feof(fptr)){
		fread(data+num,sizeof(data),1,fptr);
		//printf("read ");
		num++;
	}*/


	printf("Server is ready to receive !!\n");

      while(1){
		/*recive from client */
                if (recvfrom(socketfd, recbuf, sizeof(*recbuf), 0, (struct sockaddr*)&client , &length) <0) {
                        perror ("could not read datagram!!");
                }
                //pid_t pid;
                //pid=fork();
char  cp[] ="127.0.0.1";
inet_pton(AF_INET, cp, &client.sin_addr);
                int aaa=1;
                if(aaa){
	                //printf("Received data form %s : %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
	                //printf("%s\n", recbuf);
	                int len=20;
					char buffer[len];
					inet_ntop(AF_INET, &(client.sin_addr), buffer, len);
					printf("%s : \n",buffer);
					strcpy(sendbuf,"ACK\n");
	    /* return to client */
	                if ((n=sendto(socketfd, sendbuf, sizeof(sendbuf), 0, (struct sockaddr*)&client, length)) < 0) {
	                        perror("Could not send datagram!!\n");
	                        printf("1:%d\n",errno);
	                }
	                printf("Can Strike Crtl-c to stop Server >>\n");
	            }
        }
}

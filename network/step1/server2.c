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


#define BUFFERSIZE 32768  //32kb
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
	struct sockaddr_in server,client;
	int socketfd, n;
	socklen_t length;
	int nbytes;
	char recbuf[BUFFERSIZE];
	char sendbuf[BUFFERSIZE];
	memset((char*)&server,0,sizeof(server));
	server.sin_family = AF_INET;    // AF_INET or PF_INET
	server.sin_port = htons(atoi(argv[1]));  //port number
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if((socketfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
		printf("socket");

	if(bind(socketfd, (struct sockaddr *) &server, sizeof(server)) == -1)
		printf("bind");

	FILE* fptr = fopen("1.mp4","rb");
	char data[1004];
	memset(data,0,sizeof(data));
  int num=0;
	while(!feof(fptr)){
		fread(data+num,sizeof(data),1,fptr);
		//printf("read ");
		num++;
	}


	printf("Server is ready to receive !!\n");

		/*recive from client */
                if ((nbytes = recvfrom(socketfd, recbuf, BUFFERSIZE, 0, (struct sockaddr*)&client , &length)) <0) {
                        perror ("could not read datagram!!");

                }
	   pid_t pid;
	   pid =fork();
	 if(pid>0){
                printf("Received data form %s : %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                printf("%s\n", recbuf);
								strcpy(sendbuf,"ACK\n");
    /* return to client */
                if ((n=sendto(socketfd, sendbuf, sizeof(sendbuf), 0, (struct sockaddr*)&client, length)) < 0) {
                        perror("Could not send datagram!!\n");

                }
		}
                printf("Can Strike Crtl-c to stop Server >>\n");
}

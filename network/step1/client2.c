#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
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



int main(int argc,char* argv[]){
  struct sockaddr_in server;
	int socketfd,n;
	socklen_t length = sizeof(struct sockaddr_in);
	int nbytes;

	char sendbuf[BUFFERSIZE];
	char recbuf[BUFFERSIZE];

	memset((char*)&server,0,sizeof(server));

	if((socketfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
		printf("socket");

				server.sin_family = AF_INET;
        server.sin_port = htons(atoi(argv[1]));
        server.sin_addr.s_addr = inet_addr("127.0.0.1");


	printf("client is ready to send !!\n");

        while (1) {
		/* send to server */
		strcpy(sendbuf,"Hello\n");
			sleep(1);
		if (sendto(socketfd, sendbuf, sizeof(sendbuf), 0, (struct sockaddr*)&server, length) < 0) {
                        perror("Could not send datagram!!\n");
                        continue;
                }

                /* receive from server */
		if ((n=nbytes = recvfrom(socketfd, recbuf, sizeof(recbuf), 0, (struct sockaddr*)&server , (socklen_t *)&length)) <0) {
                        perror ("could not read datagram!!");
                        continue;
                }
		printf("Received data form %s : %d\n", inet_ntoa(server.sin_addr), htons(server.sin_port));
                printf("%s\n", recbuf);

                printf("Can Strike Crtl-c to stop Server >>\n");
        }
}

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
#include <time.h>


typedef struct TCP_header{
	uint32_t seq;
	uint32_t ACK;
	int request;
	int finish;
	uint16_t source_port;
	uint16_t dest_port;
}TCP_header;                    //5*4 = 20 bytes
typedef struct segment{
	struct TCP_header head;
	char data[1004];

}segment;
typedef struct req{
	unsigned request:5;
}Req;

int main(int argc,char* argv[]){
  struct sockaddr_in server;
	static int socketfd;
	int n;
	int length = sizeof(struct sockaddr_in);
	int nbytes;
	Req req;req.request=0;
	int cli_seq,cli_ack;
	for(int i=4; argc-i>0; i++){
		int a = atoi(argv[i]);
		int two=1;
		for(int x=0;x<a;x++) two*=2;
		req.request = req.request|(two);
	}req.request >>= 1;

	memset((char*)&server,0,sizeof(server));
		server.sin_addr.s_addr =inet_addr(argv[1]);
		server.sin_family = AF_INET;
    server.sin_port = atoi(argv[2]);
    if((socketfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
		printf("socket");

		srand(time(NULL));
		struct segment *pack = (struct segment*) malloc (sizeof(struct segment));
		pack->head.seq = (rand()%10000);
		pack->head.ACK = 0;
		pack->head.dest_port =  server.sin_port;
	printf("=====Start the three-way hand shake=====\n");
		if (sendto(socketfd, pack, 1024, 0, (struct sockaddr*)&server, length) < 0) {
		        perror("Could not send datagram!!\n");
		        printf("1:%d\n",errno);
    }
		printf("Send a packet(SYN) to %s : %d\n",argv[1], server.sin_port);
		memset(pack,0,sizeof(segment));
		if ((n=nbytes = recvfrom(socketfd,pack , 1024, 0, (struct sockaddr*)&server , (socklen_t *)&length)) <0) {
				 			                        perror ("could not read datagram!!");
		}
								int len=20;
							  char buffer[len];
							  inet_ntop(AF_INET, &(server.sin_addr), buffer, len);
							  printf("Recive a packet(SYN/ACK) from %s : %d\n",buffer, server.sin_port);
							  printf("\t Recive a packet (seq_num = %u, ack_num = %u)\n",pack->head.seq,pack->head.ACK);
								if(pack->head.seq != 0 ) printf("Recive non SYN/ACK\n");


							  cli_seq = pack->head.ACK;
								cli_ack = pack->head.seq+1;
								memset(pack,0,sizeof(segment));
								pack->head.request = req.request;
								pack->head.ACK = cli_ack;
								pack->head.seq = cli_seq;
		if (sendto(socketfd, pack, 1024, 0, (struct sockaddr*)&server, length) < 0) {
		        perror("Could not send datagram!!\n");
		        printf("1:%d\n",errno);
    }
		printf("Send a packet(ACK) to %s : %d\n",buffer,server.sin_port);
		printf("=====Complet the three-way hand shake=====\n");

		struct timeval nNewTimeout ;

		setsockopt(socketfd,SOL_SOCKET,SO_RCVTIMEO, (char *)&nNewTimeout,sizeof(struct timeval));
		for(int i=1,x=0;i<16;i*=2){
			x++;
		if(req.request & i) {
				char file[10];
				sprintf(file,"%d",x);
				strcat(file,"_copy.mp4");
				FILE* fptr1 = fopen(file,"wb");
				while(1){
					memset(pack,0,sizeof(segment));
					nNewTimeout.tv_sec = 10000;
					//recive data
					if ((n=nbytes = recvfrom(socketfd, pack, 1024, 0, (struct sockaddr*)&server , (socklen_t *)&length)) <0) perror ("could not read datagram!!");
					printf("Received data form %s : %d\n", inet_ntoa(server.sin_addr), htons(server.sin_port));
					printf("\tReceive a packet (seq_num = %d, ack_num = %d)\n",pack->head.seq,pack->head.ACK);

					if(pack->head.seq != cli_ack) {//wrong seq
						printf("Packet loss ack_num is %d\n",cli_ack);
						memset(pack,0,sizeof(segment));
						pack->head.seq=cli_seq;
						pack->head.ACK=cli_ack;
						if (sendto(socketfd, pack, 1024, 0, (struct sockaddr*)&server, length) < 0) {
									        perror("Could not send datagram!!\n");
									        printf("1:%d\n",errno);
						}
					}
					else{//right seq
						int f=0;
						f = pack->head.finish;
						cli_seq = pack->head.ACK;
						cli_ack = pack->head.seq+1;
						int num=0;
						if(!f){
							while(num<1004){
								fwrite(pack->data+num,sizeof(char),1,fptr1);
								num++;
							}
						}
						nNewTimeout.tv_usec = 20000; //delay ack
						setsockopt(socketfd,SOL_SOCKET,SO_RCVTIMEO, (char *)&nNewTimeout,sizeof(struct timeval));
						if ((n=nbytes = recvfrom(socketfd, pack, 1024, 0, (struct sockaddr*)&server , (socklen_t *)&length)) <0) {
							//timeout
							memset(pack,0,sizeof(segment));
							pack->head.seq=cli_seq;
							pack->head.ACK=cli_ack;
							if (sendto(socketfd, pack, 1024, 0, (struct sockaddr*)&server, length) < 0) {
										        perror("Could not send datagram!!\n");
										        printf("1:%d\n",errno);
							}
							if(f) break;
						}
					  else{//in time recive data
							printf("Received data form %s : %d\n", inet_ntoa(server.sin_addr), htons(server.sin_port));
							printf("\tReceive a packet (seq_num = %d, ack_num = %d)\n",pack->head.seq,pack->head.ACK);
							if(pack->head.seq!=cli_ack){//wrong seq
								printf("Packet loss ack_num is %d\n",cli_ack);
								memset(pack,0,sizeof(segment));
								pack->head.seq=cli_seq;
								pack->head.ACK=cli_ack;
								if (sendto(socketfd, pack, 1024, 0, (struct sockaddr*)&server, length) < 0) {
											        perror("Could not send datagram!!\n");
											        printf("1:%d\n",errno);
								}
							}
							else{//right seq
								f=0;
								f = pack->head.finish;
								cli_seq = pack->head.ACK;
								cli_ack = pack->head.seq+1;
								pack->head.seq=cli_seq;
								pack->head.ACK=cli_ack;
								if (sendto(socketfd, pack, 1024, 0, (struct sockaddr*)&server, length) < 0) {
											        perror("Could not send datagram!!\n");
											        printf("1:%d\n",errno);
								}
								printf("send (%d)\n",cli_ack);
								if(f) break;
								num=0;
								while(num<1004){
									fwrite(pack->data+num,sizeof(char),1,fptr1);
									num++;
								}
							}
						}
					}
			}fclose(fptr1);
		}
	}
}

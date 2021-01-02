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

char video1[123*1004],video2[45*1004],video3[257*1004],video4[204*1004];
int portlist[200];

typedef struct argu{
	struct sockaddr_in cli;
	int seq;
}argu;
struct argu queue[250];
void enqueue(struct sockaddr_in a,int seq);
struct argu dequeue();
int head,tail;
void transmit(int fd,int ser_seq,int ser_ack,int video,struct sockaddr_in client);

int port;

void* child(){
	struct sockaddr_in server,client;int length=sizeof(struct sockaddr);
	int se;
	while(1){
				while(1){
					sleep(1);
					if(head==tail) {continue;}
					struct argu tem =	dequeue();
					memcpy(&client,&tem.cli,sizeof(struct sockaddr_in));
					se=tem.seq;
					break;
				}       Req req;req.request=0;
								int sockfd, n;
								int nbytes;
								memset(&server, 0, sizeof(server));
								if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
									printf("socket");
								int addport;
								for(int i=1;i<200;i++){
									if(portlist[i]==0) {portlist[i]=1;addport=i;break;}
								}
								server.sin_family = AF_INET;
								server.sin_port = port+addport;
								server.sin_addr.s_addr = INADDR_ANY;
								if(bind(sockfd, (struct sockaddr *) &server, sizeof(server)) == -1)
									printf("bind\n");
								printf("=====Start the three-way handshake=====\n");
												uint32_t ser_seq = se;
												uint32_t ser_ack = 0;
												int len=20;
												char buffer[len];
												inet_ntop(AF_INET, &(client.sin_addr), buffer, len);
												printf("Recive a packet(SYN) from %s : %d\n",buffer, client.sin_port);
												printf("\t Recive a packet (seq_num = %u, ack_num = %u)\n",ser_seq,ser_ack);
								/* return to client */
												struct segment *pack = (struct segment *) malloc (sizeof(struct segment));
												memset(pack,0,sizeof(struct segment));
												pack->head.seq = 0;
												pack->head.ACK = se+1;
												pack->head.finish = 0;
								if ((n=sendto(sockfd, pack, 1024, 0, (struct sockaddr*)&client, length)) < 0) perror("Could not send datagram!!\n");

												printf("Send a packet(SYN/ACK) to %s : %d\n",buffer,client.sin_port);
												memset(pack,0,sizeof(struct segment));
								if (recvfrom(sockfd, pack, 1024, 0, (struct sockaddr*)&client , &length) <0) perror ("could not read datagram!!\n");
												ser_seq = pack->head.ACK;
												ser_ack = pack->head.seq+1;
												req.request = pack->head.request;
								printf("Recive a packet(ACK) from %s : %d\n",buffer, client.sin_port);
								printf("\t Recive a packet (seq_num = %d, ack_num = %d)\n",pack->head.seq,pack->head.ACK);
								printf("=====Complet the three-way handshake=====\n");
								/*start to transmit video*/
								int x=1;
								for(int i=1;i<16;i*=2){
									if(req.request & i){
										transmit(sockfd,ser_seq,ser_ack,x,client);
									}
									x++;
								}
								portlist[addport]=0;
				}
}
int main(int argc,char* argv[]){
	head=0;tail=0;
	struct sockaddr_in Server,client;
	int socketfd;
	int length=sizeof(struct sockaddr);
	memset(portlist,0,sizeof(portlist));
	port=atoi(argv[1]);
	memset(&Server, 0, sizeof(Server));
	memset(&client, 0, sizeof(client));
	if((socketfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
		printf("socket");
	Server.sin_family = AF_INET;
	Server.sin_port = port;
	Server.sin_addr.s_addr = INADDR_ANY;
	if(bind(socketfd, (struct sockaddr *) &Server, sizeof(Server)) == -1)
		printf("bind\n");
  //prepare data
	FILE* fptr1 = fopen("1.mp4","rb");
	FILE* fptr2 = fopen("2.mp4","rb");
	FILE* fptr3 = fopen("3.mp4","rb");
	FILE* fptr4 = fopen("4.mp4","rb");
	memset(video1,0,sizeof(video1));
	memset(video2,0,sizeof(video2));
	memset(video3,0,sizeof(video3));
	memset(video4,0,sizeof(video4));
	int num=0;
	while(!feof(fptr1)){
  	fread(video1+num,sizeof(char),1,fptr1);num++;
}
	num=0;
	while(!feof(fptr2)){
  	fread(video2+num,sizeof(char),1,fptr2);num++;
}
	num=0;
	while(!feof(fptr3)){
  	fread(video3+num,sizeof(char),1,fptr3);num++;
}
	num=0;
	while(!feof(fptr4)){
  	fread(video4+num,sizeof(char),1,fptr4);num++;
}

pthread_t t[5];
for(int i=0;i<5;i++)
	pthread_create(t+i,NULL,child,NULL);

  printf("============================\n");
	printf("Listening for client...\n");
		/*recive SYN from client */
	 segment *pack = (segment*) malloc (sizeof(segment));
while(1){
  		if (recvfrom(socketfd, pack, 1024, 0, (struct sockaddr*)&client , &length) <0)
                        perror ("could not read datagram!!");
									if(pack->head.ACK == 0)
											enqueue(client,pack->head.seq);
									memset(pack,0,sizeof(segment));
		}
				return 0;
}
void enqueue(struct sockaddr_in a, int seq){
			memcpy(&queue[tail].cli,&a,sizeof(struct sockaddr_in));
			queue[tail].seq=seq;
		  tail++;

}
struct argu dequeue(){
			struct argu tem;
			memset(&tem,0,sizeof(struct argu));
			if(head==tail) return tem;
			memcpy(&tem,queue+head,sizeof(struct argu));
			head++;
			return tem;
}
void transmit(int fd,int ser_seq,int ser_ack,int video,struct sockaddr_in client){
		struct segment *pack = (struct segment *) malloc (sizeof(struct segment));
		memset(pack,0,sizeof(segment));
		int length=sizeof(struct sockaddr);
		int len=20,n;
		char buffer[len];
		inet_ntop(AF_INET, &(client.sin_addr), buffer, len);
		int Max_ack=1;int Max_seq=ser_ack -1;
		struct timeval nNewTimeout ;
		nNewTimeout.tv_usec = 1;
		setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO, (char *)&nNewTimeout,sizeof(struct timeval));
		int num=0;int times=0;
		srand(time(NULL));
	switch (video){
		case 1:
			num=0;times=0;
			while(1){
				printf("Max_ack=%d\n",Max_ack);
				ser_seq = Max_ack;
				ser_ack = Max_seq+1;
				num = (Max_ack-1) * 1004;
				times = Max_ack;
				for(int z=0;z<4;z++){
					memset(pack,0,sizeof(segment));
					int i=0;
					if(times<124){
					while(i<1004){
						pack->data[i]=video1[num];num++;i++;
						}
					}
					times++;
					pack->head.seq=ser_seq++;
					pack->head.ACK=ser_ack++;
					if(times>=124)
						pack->head.finish=1;
					else
						pack->head.finish=0;
					printf("cwnd = 4 \t\t");
					int loss = (rand()%10);
					if(loss != 1){
						if ((n=sendto(fd, pack, 1024, 0, (struct sockaddr*)&client, length)) < 0) perror("Could not send datagram!!\n");
						else
							{memset(pack,0,sizeof(segment));printf("Send a packet to %s : %d (seq_num=%d, ack_num=%d)\n",buffer,client.sin_port,ser_seq-1,ser_ack-1);}
					}
					if(times>125) break;
			 }

			 		ser_seq--;
					ser_ack--;
					for(int i=0;i<4;i++){
						if (recvfrom(fd, pack, 1024, 0, (struct sockaddr*)&client , &length) <0) {
							//timeout
							printf("No recive ACK %d\n",ser_seq+1);
							continue;
						}
							if(Max_ack <= pack->head.ACK){
								Max_ack = pack->head.ACK;
								Max_seq = pack->head.seq;
							}
						printf("Recive a packet(ACK) from %s : %d\n",buffer, client.sin_port);
						printf("\t Recive a packet (seq_num = %d, ack_num = %d)\n",pack->head.seq,pack->head.ACK);
						if(pack->head.ACK == ser_seq + 1) break;
					}
						if(Max_ack >= 125) break;
		}printf("=============transmit Complet=============\n");
		 break;
		case 2:
		num=0;times=0;
		while(times<47){
				memset(pack,0,sizeof(segment));
				int i=0;
				while(i<1004){
					pack->data[i]=video2[num];num++;i++;
				}
				pack->head.seq=ser_seq;
				pack->head.ACK=ser_ack;
				if(times==46)
					pack->head.finish=1;
				else
					pack->head.finish=0;
				if ((n=sendto(fd, pack, 1024, 0, (struct sockaddr*)&client, length)) < 0) perror("Could not send datagram!!\n");
								printf("Send a packet to %s : %d\n",buffer,client.sin_port);
								memset(pack,0,sizeof(segment));
				if(times==46) break;
				if (recvfrom(fd, pack, 1024, 0, (struct sockaddr*)&client , &length) <0) perror ("could not read datagram!!\n");
								ser_seq = pack->head.ACK;
								ser_ack = pack->head.seq+1;

				printf("Recive a packet(ACK) from %s : %d\n",buffer, client.sin_port);
				printf("\t Recive a packet (seq_num = %d, ack_num = %d)\n",pack->head.seq,pack->head.ACK);
				times++;
	} break;
		case 3:
		num=0;times=0;
		while(times<259){
				memset(pack,0,sizeof(segment));
				int i=0;
				while(i<1004){
					pack->data[i]=video3[num];num++;i++;
				}
				pack->head.seq=ser_seq;
				pack->head.ACK=ser_ack;
				if(times==258)
					pack->head.finish=1;
				else
					pack->head.finish=0;
				if ((n=sendto(fd, pack, 1024, 0, (struct sockaddr*)&client, length)) < 0) perror("Could not send datagram!!\n");
								printf("Send a packet to %s : %d\n",buffer,client.sin_port);
								memset(pack,0,sizeof(segment));
								if(times==258) break;
				if (recvfrom(fd, pack, 1024, 0, (struct sockaddr*)&client , &length) <0) perror ("could not read datagram!!\n");
								ser_seq = pack->head.ACK;
								ser_ack = pack->head.seq+1;

				printf("Recive a packet(ACK) from %s : %d\n",buffer, client.sin_port);
				printf("\t Recive a packet (seq_num = %d, ack_num = %d)\n",pack->head.seq,pack->head.ACK);
				times++;
	} break;
		case 4:
		num=0;times=0;
		while(times<206){
				memset(pack,0,sizeof(segment));
				int i=0;
				while(i<1004){
					pack->data[i]=video4[num];num++;i++;
				}
				pack->head.seq=ser_seq;
				pack->head.ACK=ser_ack;
				if(times==205)
					pack->head.finish=1;
				else
					pack->head.finish=0;
				if ((n=sendto(fd, pack, 1024, 0, (struct sockaddr*)&client, length)) < 0) perror("Could not send datagram!!\n");
								printf("Send a packet to %s : %d\n",buffer,client.sin_port);
								memset(pack,0,sizeof(segment));
								if(times==205) break;
				if (recvfrom(fd, pack, 1024, 0, (struct sockaddr*)&client , &length) <0) perror ("could not read datagram!!\n");
								ser_seq = pack->head.ACK;
								ser_ack = pack->head.seq+1;

				printf("Recive a packet(ACK) from %s : %d\n",buffer, client.sin_port);
				printf("\t Recive a packet (seq_num = %d, ack_num = %d)\n",pack->head.seq,pack->head.ACK);
				times++;
	} break;
}
}

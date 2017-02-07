#include<bits/stdc++.h>
#include"my_socket.h"

using namespace std;

struct frame{
	char data[256];
	int seq;
};

int main(int argc, char* argv[]){
int sockfd = host_server(atoi(argv[1]));
int newsockfd = accept_connection(sockfd);
int s = 0;
int nn;
while(1){
	frame f;
	printf("waiting for seq = %d\n",s);
	if(read(newsockfd,(char*)&f,sizeof(f))<=0){ printf("client disconnected..\n"); exit(1);}	
	printf("frame receiving..%s\n",f.data);
	printf("want to drop the frame(1/0):");
	scanf("%d",&nn);
	if(nn) continue;
	printf("Frame successfully received: %s seq = %d\n",f.data,f.seq);
	if(f.seq != s){
		printf("Wrong frame received..\n"); 
		char ack[100];
		sprintf(ack,"%d",s);
		//strcpy(ack+10,"Piggybacking....\0");
		printf("sending ACK %d\n",atoi(ack));
		write(newsockfd,ack,sizeof(ack));
		continue;
	}
	printf("sending ACK...\n");
	char ack[100];
	s = s+1;
	s = s%2;
	sprintf(ack,"%d",s);
	//strcpy(ack+10,"Piggybacking....\0");
	printf("want to drop the ack(1/0):");
	scanf("%d",&nn);
	if(!nn) write(newsockfd,ack,sizeof(ack));

}



}

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
while(1){
	frame f;
	read(newsockfd,(char*)&f,sizeof(f));	
	printf("frame received...%s\n",f.data);
	if(f.seq != s){
		printf("Wrong frame received..\n"); 
		char ack[100];
		sprintf(ack,"%d",s);
		write(newsockfd,ack,sizeof(ack));
	}
	printf("want to send ack(1/0):");
	int nn;
	scanf("%d",&nn);
	char ack[100];
	s = s+1;
	s = s%2;
	sprintf(ack,"%d",s);
	write(newsockfd,ack,sizeof(ack));

}



}

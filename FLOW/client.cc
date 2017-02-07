#include<bits/stdc++.h>
#include"my_socket.h"

using namespace std;

struct frame{
	char data[256];
	int seq;
};

int main(int argc, char* argv[]){
if(argc<3){printf("usage %s <ip> <port>\n",argv[0]);exit(1);}
int sockfd = connect_server(argv[1],atoi(argv[2]));
int n = 1;
int s = 0;

struct timeval tv;
tv.tv_sec = 3;
tv.tv_usec = 0;
fd_set fds;

while(1){
	char msg[100];
	sprintf(msg,"this is frame no: %d", n);
	frame f;
	strcpy(f.data,msg);
	f.seq = s;
	write(sockfd,&f,sizeof(f));
	char ack[100];

	//preparing for timeout..
	FD_ZERO(&fds);
	FD_SET(sockfd,&fds);
	if(select(sockfd+1,&fds,NULL,NULL,&tv)){
		read(sockfd,ack,sizeof(ack));
		n++;
		s = s+1;
		s = s%2;
	}
	else{
		printf("timeout resending..\n");
	}

}
return 0;
}




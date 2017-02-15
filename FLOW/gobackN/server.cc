#include <bits/stdc++.h>
#include "my_socket.h"
#include <pthread.h>
#include <sys/time.h>


using namespace std;
struct frame{
	int frame_no;
	int seq_no;
};

#define rn (double)rand()/(double)RAND_MAX

int main(int argc, char* argv[]){

	if(argc < 2){printf("usage %s <port>\n", argv[0]); exit(1);}
	int sockfd = host_server(atoi(argv[1]));
	int newsockfd = accept_connection(sockfd);
	if(newsockfd < 0) perror("accept error");
	char str[100];
	read(newsockfd,str,sizeof(str));
	int seq_size = atoi(str);
	printf("sequence size = %d synchronised...\n",seq_size);
	int seq_ptr = 0;
	//while(1);
	while(1){
		frame f;
		read(newsockfd,(void*)&f,sizeof(f));
		if(f.seq_no == seq_ptr){ 
			printf("frame no %d  with seq_no = %d received\n",f.frame_no,f.seq_no);
			seq_ptr++;
			seq_ptr = seq_ptr%seq_size;
			char ack[100];
			sprintf(ack,"%d",seq_ptr);
			int r  = rn*10;
			if(r<=2){// sends ACK to 20% of packets...
				printf("sending ACK...\n");
				write(newsockfd,ack,sizeof(ack)); 
			} 
			else printf("ACK dropping...\n");
		}
		else{
			printf("frame no %d received with seq_no = %d ignoring it waiting for seq %d...\n", f.frame_no,f.seq_no,seq_ptr);
			char ack[100];
			sprintf(ack,"%d",seq_ptr);
			write(newsockfd,ack,sizeof(ack));
		}

	}

	return 0;
}

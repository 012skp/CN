#include <bits/stdc++.h>
#include "my_socket.h"
#include <pthread.h>
#include <unistd.h>

#define MAX_SEQ 100
using namespace std;
int timeout;
int td;
int f_size;
int win_size;
int seq_size;
int seq_ptr;
int win_ptr;
int ack_ptr;
pthread_t t1,t2;
int fno;
int sockfd;
int right_end;
struct window{
	int frame_no;
	struct timeval tv;
	int new_;

};


struct frame{
	int frame_no;
	int seq_no;
};

struct window[MAX_SEQ];


int mod(int a, int b){
	if(a > b) return a%b;
	if(a < 0) return a+b;
}




void* recv_ack(void* arg){
	struct timeval tv;
	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	while(1){
		if(window[ack_ptr].new_){// if this is new data wait for it...
			select(sockfd)
		}

	}

}

int flag;

void* sends_frame(void* arg){
	while(1){

		while(win_ptr != right_end){
			struct frame f;
			f.seq_no = seq_ptr;
			f.frame_no = fno;
			window[win_ptr].frame_no = fno;
			gettimeofday(&(window[s].tv),NULL);
			write(sockfd,(void*)&f, sizeof(f));
			window[win_ptr].new_ = 1;
			if(flag) {pthread_create(&t1,NULL,recv_ack,NULL); flag = 0; ack_ptr = seq_ptr; }
			printf("frame %d with seq = %d send at time = %ld sec %ld usec\n",fno,seq_ptr,f.tv.tv_sec,f.tv.tv_usec);
			usleep(td); //sleep for transmission delay...
			fno++;
			seq_ptr++;
			seq_ptr = mod(seq_ptr,seq_size);
			win_ptr++;
			win_ptr = mod(win_ptr,win_size);
		}

		//window size zero...
		//wait for window to expand....
		printf("window size is zero waiting for window to expand....\n");
		usleep(500000);
	}
}

int main(int argc, char* argv[]){
	for(int i=0;i<MAX_SEQ;i++) { window[i].tv.tv_sec = 0; window[i].tv.tv_usec = 0; window[i].new_ = 0; }
	if(argc < 3){printf("usage %s <ip> <port>\n",argv[0]); exit(1);}

	int sockfd = connect_server(argv[1],atoi(argv[2]));

	printf("enter window_size:");
	cin>>win_size;
	win_size--;
	seq_size = win_size+1;
	printf("enter frame_size(in Kb):");
	cin>>f_size;

	int bandwidth;
	printf("enter bandwidth(in Kbps):");
	cin>>bandwidth;

	double transmission_delay = (double)f_size/(double)bandwidth); //in seconds...
	td = transmission_delay*1000000; //in usec....

	
	printf("enter timeout(in seconds):");
	cin>>timeout;


	fno = 0;
	win_ptr = 0;
	right_end = win_size;
	seq_ptr = 0;
	pthread_create(&t2,NULL,sends_frame,NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	return 0;
}

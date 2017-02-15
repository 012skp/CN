#include <bits/stdc++.h>
#include "my_socket.h"
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

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
int flag;


pthread_mutex_t lock;

struct buff_window{
	int frame_no;
	struct timeval tv;
	int new_;
	int seq_no;
};


struct frame{
	int frame_no;
	int seq_no;
};

struct buff_window window[MAX_SEQ];


int mod(int a, int b){
	if(a >= b) return a%b;
	if(a < 0) return a+b;
}


void* sends_frame(void* );

void* recv_ack(void* arg){
	struct timeval tv;
	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	while(1){
		if(window[ack_ptr].new_ == 1){// if this is new data wait for ACK for it...
			//set timeout...
			printf("\t\t\t\twaiting for ACK for seq_no = %d\n",window[ack_ptr].seq_no);
			struct timeval tv1,tv2,tv3;
			tv3.tv_sec = timeout;
			tv3.tv_usec = 0;
			gettimeofday(&tv1,NULL);
			timersub(&tv1,&(window[ack_ptr].tv),&tv2); 	// tv2 = time frame has already passed..
			timersub(&tv3,&tv2,&tv1);					// tv1 = timeout for this frame...
			fd_set fds;
			FD_ZERO(&fds);
			FD_SET(sockfd,&fds);
			if(select(sockfd+1,&fds,NULL,NULL,&tv1)){
					char ack[100];
					read(sockfd,ack,sizeof(ack));
					//check whether this ack matches with any frame that has been sent till this time.
					int ack_for = atoi(ack);
					ack_for--;
					ack_for = mod(ack_for,seq_size);

					if(ack_for == window[ack_ptr].seq_no -1){// receives ACK for failed duplicated data, ignore it...
						continue;
					} 
					int ff = 0;
					while(1){
						if(window[ack_ptr].new_ == 0){ ff = 1; break;}
						window[ack_ptr].new_ = 0; //set this frame is old, it means this can be overwritten...
						if(window[ack_ptr].seq_no == ack_for){
							printf("\t\t\t\tack received for seq_no = %d,\n",ack_for);
							window[ack_ptr].new_ = 0;
							ack_ptr++;
							ack_ptr = mod(ack_ptr,win_size);
							right_end++;						//increase the window size...
							right_end = mod(right_end,win_size);
							break;
						}
						window[ack_ptr].new_ = 0;
						ack_ptr++;
						ack_ptr = mod(ack_ptr,win_size);
						right_end++;						//increase the window size...
						right_end = mod(right_end,win_size);

					}
					if(ff){
						printf("ack received....\n");
						continue;
					} 					
			}
			else{ //timeout so send everything from ack_ptr again...
				pthread_mutex_lock(&lock);
				printf("\t\t\t\tTIMEOUT couldn't receive the ACK for seq_no = %d\n",window[ack_ptr].seq_no);
				window[ack_ptr].new_ = 0;
				int tt = ack_ptr+1;
				while(tt != ack_ptr){
					window[tt].new_ = 0;
					tt++;
					tt = mod(tt,win_size);
				}
				win_ptr = ack_ptr;
				right_end = ack_ptr;	//make window size full...
				right_end = mod(right_end,win_size);
				seq_ptr = window[ack_ptr].seq_no;
				fno = window[ack_ptr].frame_no;
				printf("\t\t\t\tgoint to send everything from begining.....\n");
				pthread_mutex_unlock(&lock);
			}

		}
		
		else {
			//printf("waiting for another thread to send frame\n");
			usleep(td);
		}

	}

}



void* sends_frame(void* arg){
	while(1){
		//printf("at win_ptr = %d and right_end = %d\n",win_ptr, right_end);
		while(win_ptr != right_end || window[win_ptr].new_ == 0){
			struct frame f;
			f.seq_no = seq_ptr;
			f.frame_no = fno;
			window[win_ptr].frame_no = fno;
			window[win_ptr].seq_no = seq_ptr;
			gettimeofday(&(window[win_ptr].tv),NULL);
			write(sockfd,(void*)&f, sizeof(f));
			window[win_ptr].new_ = 1;
			if(flag) {pthread_create(&t1,NULL,recv_ack,NULL); flag = 0; ack_ptr = win_ptr; }
			printf("frame %d with seq = %d  at win_ptr = %d send at time = %ld sec %ld usec WIN_SIZE = %d\n",
				fno,seq_ptr,win_ptr,window[win_ptr].tv.tv_sec,window[win_ptr].tv.tv_usec,
				(right_end==win_ptr?win_size: (win_ptr<right_end?right_end - win_ptr: (win_size - (win_ptr - right_end) ) )));
			usleep(td); //sleep for transmission delay...
			fno++;
			seq_ptr++;
			seq_ptr = mod(seq_ptr,seq_size);
			win_ptr++;
			win_ptr = mod(win_ptr,win_size);
		}

		//window size zero...
		//wait for window to expand....
		printf("window size is zero waiting for window to expand at win_ptr = %d  right_end = %d\n",win_ptr,right_end);
		usleep(500000);
	}
}

int main(int argc, char* argv[]){
	for(int i=0;i<MAX_SEQ;i++) { window[i].tv.tv_sec = 0; window[i].tv.tv_usec = 0; window[i].new_ = 0; }
	if(argc < 3){printf("usage %s <ip> <port>\n",argv[0]); exit(1);}

	sockfd = connect_server(argv[1],atoi(argv[2]));

	printf("enter window_size:");
	cin>>win_size;
	seq_size = win_size+1;
	printf("enter frame_size(in Kb):");
	cin>>f_size;

	int bandwidth;
	printf("enter bandwidth(in Kbps):");
	cin>>bandwidth;

	double transmission_delay = (double)f_size/(double)bandwidth; //in seconds...
	td = transmission_delay*1000000; //in usec...

	
	printf("enter timeout(in seconds):");
	cin>>timeout;


	fno = 0;
	win_ptr = 0;
	right_end = 0;
	seq_ptr = 0;
	flag = 1;
	char str[100];
	sprintf(str,"%d",seq_size);
	write(sockfd,str,sizeof(str));
	pthread_create(&t2,NULL,sends_frame,NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	return 0;
}

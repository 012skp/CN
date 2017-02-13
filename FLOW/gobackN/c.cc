#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/time.h>


using namespace std;

int main(){

	struct timeval tv1,tv2,tv3;
	tv1.tv_sec = 5;
	tv1.tv_usec = 0;
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0,&fds);
	gettimeofday(&tv2,NULL);
	if(select(1,&fds,NULL,NULL,&tv1)) printf("key pressed before timeout\n");
	else printf("time out 5 second...\n");
	gettimeofday(&tv3,NULL);

	printf("time diff = %ld second  %ld usec\n",tv3.tv_sec-tv2.tv_sec, tv3.tv_usec-tv2.tv_usec );
	return 0;
}
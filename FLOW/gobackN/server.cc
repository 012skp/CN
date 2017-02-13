#include <bits/stdc++.h>
#include "my_socket.h"
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>



#define MAX_SEQ 100
using namespace std;


int seq[MAX_SEQ];



int main(int argc, char* argv[]){

	struct timeval tv1,tv2;
	gettimeofday(&tv1,NULL);
	int n = 10000000;
	while(n--);
	gettimeofday(&tv2,NULL);
	printf("time diff = %ld sec %ld usec\n",(long int)tv2.tv_sec-tv1.tv_sec,  (long int)tv2.tv_usec-tv1.tv_usec );

	int a;
	long b;
	long long c;


	return 0;
}

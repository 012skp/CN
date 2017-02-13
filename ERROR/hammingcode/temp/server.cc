#include<bits/stdc++.h>
#include"my_socket.h"
//server side...
using namespace std;



int check_error(char str[]){
int n = strlen(str)-1;
int rbit = ceil(log2(n));
int err_pos = 0;

  for(int i=0;i<rbit;i++){
          int idx = pow(2,i);
	  int pidx = idx;
          int pos = 0;
          int sum = 0;
          while(1){
                  for(int j=idx;j<=n && j<idx+pidx;j++){
                          sum = sum^( ((int)str[j]) -48);
			 // printf("taking xor with %d\n",j);
                  }
                  idx = idx+2*pow(2,i);
                  if(idx > n) break;
          }
          if(sum ==1){
		err_pos += pow(2,i);
		}
  }
return err_pos;

}


void extract_data(char str[],char buff[]){
int n = strlen(buff)-1;
int c = 0;
int idx = 0;
int check = pow(2,c);
for(int i=1;i<=n;i++){
        if(i==check){ c++; check = pow(2,c); continue;}
        else{
		//printf("putting %d into str from buff\n",i);
                str[idx] = buff[i];
                idx++;
        }
}

}




int main(int argc, char* argv[]){
if(argc<2) exit(1);
int sockfd = host_server(atoi(argv[1]));
int newsockfd = accept_connection(sockfd);

char buff[256];
read(newsockfd,buff,sizeof(buff));
printf("data received: %s\n",buff+1);
int errbit = check_error(buff);
if(errbit){
	printf("Error in bit %d\n",errbit);
	if(buff[errbit] == '0') buff[errbit] = '1';
	else buff[errbit] = '0';
}
else printf("No error detected..\n");
char data[256];
bzero(data,sizeof(data));
extract_data(data,buff);
printf("data is %s\n",data);

return 0;
}





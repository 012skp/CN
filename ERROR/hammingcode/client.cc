#include<bits/stdc++.h>
#include"my_socket.h"

using namespace std;

int calc_bits(int msg_size){
int i=0;
while(pow(2,i)-i-1 < msg_size) i++;
return i;
}

void fill_data(char str[],int n,char buff[]){
int c = 0;
int idx = 0;
int check = pow(2,c);
for(int i=1;i<=n;i++){
	if(i==check){ str[i] = '0'; c++; check = pow(2,c); continue;}
	else{
		str[i] = buff[idx];
		idx++;
	}
}

}


void fill_redbits(char str[], int n, int rbit){
for(int i=0;i<rbit;i++){
	int idx = pow(2,i);
	int pidx = idx;
	int pos = 0;
	int sum = 0;
	while(1){
		//printf("idx = %d\n",idx);
		for(int j=idx;j<=n && j<idx+pidx;j++){
			sum = sum^( ((int)str[j]) -48);
			//printf("taking xor with %d\n",j);
		}
		idx = idx+2*pow(2,i);
		//printf("now idx = %d\n",idx);
		if(idx > n) break;
	}
	str[(int)pow(2,i)] = ((char)sum)+48;
	int pp = (int)pow(2,i);
	//printf("str[%d] = %c\n",pp,str[pp]);
}

}


int main(int argc, char* argv[]){
if(argc < 3){printf("usage %s <ip> <portno> \n",argv[0]); exit(1);}
int sockfd = connect_server(argv[1],atoi(argv[2]));

char buff[256];
scanf("%s",buff);
int msg_size = strlen(buff);
int bits_req = calc_bits(msg_size);
int total_size = bits_req+msg_size;
char str[total_size+2];
str[0] = '$';
printf("buff = %s \n",buff);
fill_data(str,total_size,buff);
str[total_size+1] = '\0';
printf("before setting redbits str = %s\n",str+1);
fill_redbits(str,total_size,bits_req);
printf("before error str = %s\n",str+1);
printf("want error(1/0):");
int nn;
scanf("%d",&nn);
if(nn){
	int r = rand()%total_size;
	r++;
	printf("Corrupting %d bit\n",r);
	if((str[r]) == '0') str[r] = '1';
	else str[r] = '0';
}
bzero(buff,sizeof(buff));
strcpy(buff,str);
printf("sending data: %s\n",buff+1);
write(sockfd,buff,sizeof(buff));

return 0;
}



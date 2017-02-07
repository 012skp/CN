#include <bits/stdc++.h>
#include "my_socket.c"
#include "gen_crc_bits.c"
using namespace std;

int main(int argc, char* argv[]){
	if(argc < 3){printf("usage %s <ip> <portno>\n",argv[0]); exit(1);}
	int sockfd = connect_server(argv[1],atoi(argv[2]));
	char gen_pol[100];
	printf("enter gen_pol:"); fflush(stdout);
	scanf("%s",gen_pol);
	write(sockfd,gen_pol,sizeof(gen_pol));
	int msg_size = pow(2,strlen(gen_pol)-1) -strlen(gen_pol);
	printf("msg per segment = %d\n",msg_size);
	char data[200];
	char msg[200];

	while(1){
		printf("enter your msg:"); fflush(stdout);
		scanf("%s",msg);
		int posm = 0;
		bzero(data,sizeof(data));
		while(1){
			if(strlen(msg+posm)<=msg_size) {
				char temp_msg[msg_size+strlen(gen_pol)];
				strcpy(temp_msg,msg+posm);
				gen_reduntant_bits(temp_msg,gen_pol);
				strcat(data,temp_msg);
				strcat(data,"E\0");
				break;
			}
			else{
				char temp_msg[msg_size+strlen(gen_pol)];
				bzero(temp_msg,sizeof(temp_msg));
				strncpy(temp_msg,msg+posm,msg_size);
				gen_reduntant_bits(temp_msg,gen_pol);
				strcat(data,temp_msg);
				strcat(data,"E\0");
				posm = posm+msg_size;
			}
			
		}
		printf("data to send = %s\n",data);
		printf("want to corrupt data(1/0):"); fflush(stdout);
		int nn;
		scanf("%d",&nn);
		if(nn){
			int r =rand()%strlen(data);
			//while(data[r] == 'E') r =  rand()%strlen(data); //Don't corrupt end tag...
			if(data[r] == '0') data[r] = '1';
			else if(data[r] == '1') data[r] = '0';
			else data[r] = '0';
		}
		printf("sending data = %s\n",data);
		write(sockfd,data,sizeof(data));
	}
	
	return 0;
}
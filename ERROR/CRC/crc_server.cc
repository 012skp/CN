#include <bits/stdc++.h>
#include "my_socket.c"
#include "gen_crc_bits.c"
using namespace std;

void detect_error(char str[], char gen_pol[],char msg[]){
	printf("detect_error in frame: %s\n",str);
	int size = pow(2,strlen(gen_pol));
	char rem[strlen(gen_pol)] = "";
	for(int i=0;i<strlen(gen_pol)-1;i++) strcat(rem,"0\0"); 
	char* token = strtok(str,"E");
	while(token != NULL){
		printf("checking error in segment:%s\n",token);
		char temp_token[size];
		strcpy(temp_token,token);
		int pos = 0;
		for(int i=0;i<strlen(temp_token)-strlen(gen_pol)+1;i++) {xor_pos(temp_token,gen_pol,pos); pos++;}

		char temp_rem[strlen(gen_pol)];
		strcpy(temp_rem,temp_token+strlen(temp_token)-strlen(gen_pol)+1);
		//printf("rem = %s temp_rem = %s\n",rem,temp_rem);
		if(!strcmp(rem,temp_rem)) printf("this segment is correct...\n");
		else {printf("this segment is corrupted..\n");}
		strncat(msg,token,strlen(token)-strlen(gen_pol)+1);
		token = strtok(NULL,"E");

	}

}


int main(int argc, char* argv[]){
	if(argc < 2){printf("usage %s <ip>\n",argv[0]); exit(1);}
	int sockfd = host_server(atoi(argv[1]));
	int newsockfd = accept_connection(sockfd);

	char str[256];
	char gen_pol[100];
	char data[200];
	read(newsockfd,str,sizeof(str));
	strcpy(gen_pol,str);
	printf("gen_pol = %s\n",gen_pol);
	while(1){
		bzero(str,sizeof(str));
		if(read(newsockfd,str,sizeof(str)) <= 0) break;
		bzero(data,sizeof(data));
		detect_error(str,gen_pol,data);
		printf("data extracted: %s\n",data);
		
	}

	return 0;
}
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include"gen_check_sum.h"


void check_error(char str[]){
char str1[13];
strncpy(str1,str,8);
gen_check_sum(str1);
char str2[5];
char str3[5];
strncpy(str2,str+8,4);
strncpy(str3,str1+8,4);
str2[4]='\0';
str3[4]='\0';
printf("checksum received: %s and checksum calculated: %s\n",str2,str3);
if(!strcmp(str2,str3)) printf("Correct data received..\n");
else printf("Data corrupted..\n");

}

int main(int argc, char *argv[]){
	
	int sockfd,newsockfd,portno,clilen;
	struct sockaddr_in serv_addr, cli_addr;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd <0){perror("Error creating socket:"); exit(1);}
	if(argc < 2){printf("usage %s <portno>\n",argv[0]); exit(1);}
	portno = atoi(argv[1]);
	
	bzero((char*)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;	
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
	if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0){
		perror("binding error:");
		exit(1);
	}
	
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	
	newsockfd = accept(sockfd,(struct sockaddr*)&cli_addr,&clilen);
	if(newsockfd < 0){perror("accept error:"); exit(1);}
	
	char str[100];
		read(newsockfd,str,12);
		str[12] = '\0';
		printf("data:%s\n",str);
		printf("Checking error...\n");
		check_error(str);


return 0;
}















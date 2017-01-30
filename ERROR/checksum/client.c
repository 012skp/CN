#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/types.h>
#include"gen_check_sum.h"


int main(int argc, char *argv[]){
if(argc < 3){printf("usage %s <ip> <portno>\n",argv[0]); exit(1);}
int sockfd,portno;
struct sockaddr_in serv_addr;
sockfd = socket(AF_INET,SOCK_STREAM,0);
if(sockfd < 0){perror("socket creation failed:"); exit(1);}
portno = atoi(argv[2]);
bzero((char*)&serv_addr,sizeof(serv_addr));
serv_addr.sin_port = htons(portno);
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0){
	perror("connection failed:"); exit(1);
}

printf("enter 8 bit data:");
char buff[100];
scanf("%s",buff);
char str[13];
strncpy(str,buff,8);
gen_check_sum(str);
printf("do you want to corrupt(1/0):");
 int n;
 scanf("%d",&n);
 if(n){
 int r = rand()%8;
 if(str[r] == '0') str[r] = '1'; 
 else if(str[r] == '1') str[r] = '0';
 }

write(sockfd,str,12);
close(sockfd);

return 0;
}




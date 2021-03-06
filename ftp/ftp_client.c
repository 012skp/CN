#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

int BLOCK_SIZE;

#define HELP printf("available commands 'ls','cd','pwd','upload <src_path> <dstn_path>','download <src_path> <dstn_path>','exit'\n");
struct data{
	long long int error;
	char function[20]; //ls or upload or download...
	char src_addr[100];
	char dstn_addr[100];
};

void get_line(char str[]){
	char ch;
	int i=0;
	while( (ch = getchar()) != '\n'){
	str[i] = ch;
	i++;
	}
	str[i] = '\0';
}

void print_status(long long int l, long long int r, int n){
	if(l>r) l =r;
	int step = n;
	int cur = ((double)l/(double)r)*step;
	printf("\r{");
	int i;
	char ch = '|', head = '>';
	for(i=1;i<cur;i++) putchar(ch);
	if(i==step) putchar(ch); else putchar(head);
	for(i=cur+1;i<=step;i++) putchar(' ');
	putchar('}');fflush(stdout);
	printf("Wait...%lld/%lld",l,r);
}

int main(int argc, char *argv[]){
	if(argc < 4){printf("usage %s hostname port BLOCK_SIZE\n",argv[0]); exit(1);}
	int sockfd,portno;
	struct sockaddr_in serv_addr;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd <0){perror("Error creating socket..\n"); exit(1);}
	portno = atoi(argv[2]);
	bzero( (char*)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	
	
	if( connect(sockfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0){ perror("Error connectin...\n"); exit(1);}
	printf("Successfully connected to server...\n");
	HELP
	struct data d1;
	d1.error = BLOCK_SIZE = atoi(argv[3]);
	char str[BLOCK_SIZE];
	write(sockfd,(void*)&d1,sizeof(d1)); //making buffer size equal for client and server...
	char cmd[20];
	while(1){
		printf("ftp:>");fflush(stdout);
		bzero(str,sizeof(str));
		get_line(str);
		if(!strncmp(str,"ls",2)){
			strcpy(d1.function,str);
			if(write(sockfd,(void*)&d1,sizeof(d1))<0){printf("Server disconnected... exiting..\n"); exit(1);}
			while(1){
				bzero(str,sizeof(str));
				if(read(sockfd,str,sizeof(str)) < 0){printf("Server disconnected... exiting..\n"); exit(1);}
				if(!strncmp(str,"DONE123",sizeof("DONE123"))){
					break;
				}
				str[sizeof(str)] = '\0';
				printf("%s",str);
			}
		}
		else if(!strcmp(str,"pwd")){
			//printf("sending '%s' command\n",str);
			strcpy(d1.function,str);
			if(write(sockfd,(void*)&d1,sizeof(d1))<0){printf("Server disconnected... exiting..\n"); exit(1);}
			bzero(str,sizeof(str));
			if(read(sockfd,str,sizeof(str)) < 0){printf("Server disconnected... exiting..\n"); exit(1);}
			printf("%s\n",str);
		}
		else if(!strncmp(str,"cd",2)){
			//printf("sending %s command\n",str);
			strcpy(d1.function,str);
			if(write(sockfd,(void*)&d1,sizeof(d1))<0){printf("Server disconnected... exiting..\n"); exit(1);}
			if(read(sockfd,(void*)&d1,sizeof(d1))<0){printf("Server disconnected....exiting...\n"); exit(1); }
			if(d1.error == 1){
				printf("INVALID DIRECTORY...\n");
				continue;
			}
			else{
				if(read(sockfd,str,sizeof(str)) < 0){printf("Server disconnected... exiting..\n"); exit(1);}
				printf("%s\n",str);
			}
		}
		else if(!strncmp(str,"download",8)){
			//printf("sending %s command\n",str);
			sscanf(str,"%s %s %s",d1.function,d1.src_addr,d1.dstn_addr);
			//printf("str = %s function = %s src_addr = %s dstn-addr = %s\n",str,d1.function,d1.src_addr,d1.dstn_addr);
			FILE *fp = fopen(d1.dstn_addr,"w");
			if(fp == NULL){perror("fopen error:"); continue;}
			if(write(sockfd,(void*)&d1,sizeof(d1))<0){printf("Server disconnected... exiting..\n"); exit(1);}
			bzero((void*)&d1,sizeof(d1));
			if(read(sockfd,(void*)&d1,sizeof(d1))<0){printf("Server disconnected... exiting..\n"); exit(1);}
			if(d1.error == 1){
				printf("Error file not found...\n");
				fclose(fp);
				unlink(d1.dstn_addr);				
				continue;
			}
			long long int sz = atoll(d1.function);
			long long int total = sz;
			printf("File size : %lld\n",sz);
			while(sz>0){
				bzero(str,sizeof(str));
                                if(read(sockfd,str,sizeof(str)) < 0){printf("Server disconnected... exiting..\n"); exit(1);}
				if(sz >= sizeof(str)) fwrite(str,sizeof(str),1,fp);
				else fwrite(str,strlen(str),1,fp);
				sz-=sizeof(str);
				print_status(total-sz,total,50);
			}
			printf("\nDONE\n");
			fclose(fp);

		}
		else if(!strncmp(str,"upload",6)){
			//printf("sending %s command\n",str);
			sscanf(str,"%s %s %s",d1.function,d1.src_addr,d1.dstn_addr);
			//printf("str = %s function = %s src_addr = %s dstn-addr = %s\n",str,d1.function,d1.src_addr,d1.dstn_addr);
			FILE *fp = fopen(d1.src_addr,"r");
			if(fp == NULL){perror("fopen error:"); continue;}
			fseek(fp,0,SEEK_END); //go to the end...
			d1.error = ftell(fp); //find flie size..
			long long sz = d1.error;
			long long total = sz;
			printf("File size : %lld\n",sz);
			rewind(fp); //rewind it to the begining...
			write(sockfd,(void*)&d1,sizeof(d1));
			read(sockfd,(void*)&d1,sizeof(d1));
			if(d1.error == 1){printf("Incorrect filename...\n"); fclose(fp); continue;}
			char str[256];
			while(!feof(fp)){
				bzero(str,sizeof(str));
				fread(str,sizeof(str),1,fp);
				write(sockfd,str,sizeof(str));
				sz-=sizeof(str);
				print_status(total-sz,total,50);
			}
			printf("\nDONE\n");
			fclose(fp);
		}
		else{printf("Invalid Command...\n"); sleep(1);} 

	}
	
	return 0;
}	

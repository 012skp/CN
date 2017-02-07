#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

int BLOCK_SIZE;

struct data{
	long long int error;
	char function[20]; //ls or upload or download...
	char src_addr[100];
	char dstn_addr[100];
};

void child_process(int newsockfd,int n){
	struct data d1;
	read(newsockfd,(void*)&d1,sizeof(d1));
	BLOCK_SIZE = d1.error;
	printf("Buffer size received = %d\n",BLOCK_SIZE);
	while(1){
		bzero(&d1,sizeof(d1));
		printf("waiting for input..\n");
		if(read(newsockfd,(void*)&d1,sizeof(d1)) > 0){
			if(!strncmp(d1.function,"ls",2)){
				FILE *fp = popen(d1.function, "r");
				if(fp == NULL){printf("popen ls failed.. exiting..\n"); exit(1);}
				char str[BLOCK_SIZE];
				while(!feof(fp)){
					bzero(str,sizeof(str));
					fread(str,sizeof(str),1,fp);
					write(newsockfd,str,sizeof(str));
				}
				write(newsockfd,"DONE123",sizeof("DONE123"));
				pclose(fp);
			}
			else if(!strncmp(d1.function,"cd",2)){
				if(chdir(d1.function+3) != 0){ //Invalid Directory...
					d1.error=1;
					write(newsockfd,(void*)&d1,sizeof(d1));
					continue;
				}
				d1.error = 0;
				write(newsockfd,(void*)&d1,sizeof(d1));
				char str[BLOCK_SIZE];
				bzero(str,sizeof(str));
				getcwd(str,sizeof(str));
				write(newsockfd,str,sizeof(str));	
			}
			else if(!strncmp(d1.function,"pwd",3)){
				char str[BLOCK_SIZE];
				bzero(str,sizeof(str));
				getcwd(str,sizeof(str));
				printf("sending %s\n",str);
				write(newsockfd,str,sizeof(str));
			}
			else if(!strncmp(d1.function,"download",8)){
				FILE *fp = fopen(d1.src_addr,"rb");
				if(fp == NULL){ 
					d1.error = 1;
					write(newsockfd,(void*)&d1,sizeof(d1));
					continue;
				}
				d1.error = 0;
				fseek(fp,0,SEEK_END); //go to the end..
				long long int sz = ftell(fp);
				rewind(fp);
				sprintf(d1.function,"%lld",sz);
				write(newsockfd,(void*)&d1,sizeof(d1));
				char str[BLOCK_SIZE];
				while(!feof(fp)){
					bzero(str,sizeof(str));
					fread(str,sizeof(str),1,fp);
					if(write(newsockfd,str,sizeof(str)) < 0){printf("Client %d disconnected..\n",n); exit(1);}
				}
				fclose(fp);
			}
			else if(!strncmp(d1.function,"upload",6)){
				char str[BLOCK_SIZE];
				long long sz = d1.error;
				FILE *fp = fopen(d1.dstn_addr,"w"); //create a new file...
				if(fp == NULL){ //Filename incorrect...
					printf(" fopen error...\n"); 
					perror("error: "); 
					d1.error = 1;
					write(newsockfd,(void*)&d1,sizeof(d1));					
					continue;
				}
				d1.error = 0;
				write(newsockfd,(void*)&d1,sizeof(d1));
				while(sz>0){
					bzero(str,sizeof(str));
		                        if(read(newsockfd,str,sizeof(str)) < 0){printf("Client %d disconnnected..\n",n); exit(1);}
					if(sz >= sizeof(str)) fwrite(str,sizeof(str),1,fp);
					else fwrite(str,strlen(str),1,fp);
					sz-=sizeof(str);
				}
				fclose(fp);
			}
			else{ printf("Something unusual command received...\n"); }
		}
		else{ printf("Client %d disconnected..\n",n); break; }
		



	}	
}


int main(int argc, char *argv[]){
	int sockfd,newsockfd,portno,clilen;
	struct sockaddr_in serv_addr,cli_addr;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	//if(sockfd <0){perror("Error creating socket..\n"); exit(1);}
	bzero( (char*)&serv_addr,sizeof(serv_addr));
	portno = atoi("4444");
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if(bind(sockfd,(struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){perror("Error binding ...\n"); exit(2);}
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	int n = 1;
	while(1){
		newsockfd = accept(sockfd,(struct sockaddr*)&cli_addr,&clilen);
		if(newsockfd < 0){perror("Error on accept...\n"); exit(3);}
		int pid = fork();
		if(pid == 0){ child_process(newsockfd,n); close(newsockfd); exit(0); }
		n++;
	}
	return 0;
}

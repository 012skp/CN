#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
int host_server(int portno){
	
	int sockfd;
	struct sockaddr_in serv_addr;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	
	bzero((char*)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;	
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
	if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0){
		perror("binding error:");
		exit(1);
	}
	
	listen(sockfd,5);

	return sockfd;
}

int accept_connection(int sockfd){
	struct  sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);
	int newsockfd = accept(sockfd,(struct sockaddr*)&cli_addr,&clilen);
	if(newsockfd < 0){perror("accept error:"); exit(1);}
	return newsockfd;
}



int connect_server(char ip[], int portno){
	int sockfd;
	struct sockaddr_in serv_addr;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0){perror("socket creation failed:"); exit(1);}

	bzero((char*)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);

	if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0){
		perror("connection failed:"); exit(1);
	}

	return sockfd;
}
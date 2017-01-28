#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>


int main( int argc, char *argv[] ) {
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n;
   
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = atoi(argv[1]);
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
      
   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */
   
   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   
   /* Accept actual connection from the client */
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);	
   if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
   }
  char sorry[10] = "sorry";
  char str[100];
  read(newsockfd,str,sizeof(str));
  printf("string received: %s\n",str);
  int i;
  int flag = 1;
  for(i=0;i<strlen(str);i++){
	//printf("%c\n",str[i]);
	if( ((int)str[i] - 48) > 10 ){ flag=0; break; }
  }
  
  
  if(flag){
	long long int nn = atoll(str);
	int sum = 0;
	while(nn>0){
	sum+= nn%10;
	nn = nn/10;
	}
	sprintf(str,"%d",sum);
	printf("sending %d\n",sum);
	write(newsockfd,str,sizeof(str));
	while(sum >= 10){
		read(newsockfd,str,sizeof(str));
		printf("string received: %s\n",str);
		nn = atoll(str);
		sum = 0;
		while(nn>0){
		sum+= nn%10;
		nn = nn/10;
		}
		sprintf(str,"%d",sum);
		printf("sending %d\n",sum);
		write(newsockfd,str,sizeof(str));
	}
  }
  else write(newsockfd,sorry,sizeof(sorry));

  close(newsockfd);
  close(sockfd);

  return 0;
}

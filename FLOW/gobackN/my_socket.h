#ifndef MY_SOCKET__H
#define MY_SOCKET__H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

int host_server(int portno);
int accept_connection(int sockfd);
int connect_server(char ip[], int portno);

#endif
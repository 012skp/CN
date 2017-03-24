#include <bits/stdc++.h>
#include "my_socket.h"

using namespace std;

struct table_row{
  int mac_addr;
  int sock_fd;
  int connected;
};
struct frame{
  int src_addr;
  int dstn_addr;
  int protocol;
};


vector<table_row> table;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
#define LOCK pthread_mutex_lock(&lock)
#define UNLOCK pthread_mutex_unlock(&lock)

void print_switch_table(){
  printf("Switch Table....\ninterface ---  MAC_address\n");
  for(int i=0;i<table.size();i++){
    if(table[i].mac_addr == -1) printf("if%d    ---  INVALID\n",i);
    else printf("if%d    ---  %d\n",i,table[i].mac_addr);
  }
}


void* thread_work(void * arg){
  int free_port = *(int *)arg;
  LOCK;
  table[free_port].connected = 1;
  UNLOCK;
  int newsockfd = table[free_port].sock_fd;
  while(1){
    frame f;
    if(read(newsockfd,(frame*)&f,sizeof(frame)) < 0){
      //Node disconnect...
      table[free_port].connected = 0;
      table[free_port].mac_addr = -1;
      print_switch_table();
      return NULL;
    }
    //If mac_addr for this is not updated...
    if(table[free_port].mac_addr == -1){ table[free_port].mac_addr = f.src_addr; print_switch_table();}
    //Look into table for dstn_addr..
    int inf = -1;
    for(int i=0;i<table.size();i++){
      if(table[i].connected && table[i].mac_addr == f.dstn_addr) inf = i;
    }
    //If found unicast otherwise broadcast...
    if(inf == -1){
      for(int i=0;i<table.size();i++){
        if(i == free_port) continue;
        if(table[i].connected) write(table[i].sock_fd,(char*)&f,sizeof(frame));
      }
    }
    else{
      write(table[inf].sock_fd,(char*)&f,sizeof(frame));
    }
  }

}


int main(int argc, char* argv[]){
  if(argc < 3){printf("usage %s <portno> <no_of_interface>\n", argv[0]); exit(1);}
  int sockfd = host_server(atoi(argv[1]));
  int n = atoi(argv[2]);
  table.resize(n);
  for(int i = 0;i< table.size();i++){ table[i].connected = 0; table[i].mac_addr = -1;}
  print_switch_table();

  //Accept connections.....
  while(1){
    int newsockfd = accept_connection(sockfd);
    printf("New incoming connection....\n");
    //If any port is free assign him....
    int free_port = -1;
    for(int i=0;i<table.size();i++)
      if(table[i].connected == 0){free_port = i; break;}

    if(free_port == -1){
      frame f;
      f.protocol = -1;
      printf("Got rejected....\n");
      write(newsockfd,(char*)&f,sizeof(frame));
    }
    else{
      frame f;
      f.protocol = 1;
      printf("Got accepted....\n");
      write(newsockfd,(char*)&f,sizeof(frame));
      LOCK;
      table[free_port].sock_fd = newsockfd;
      UNLOCK;
      pthread_t t1;
      pthread_create(&t1,NULL,thread_work,(void*)&free_port);
    }

  }




  return 0;
}

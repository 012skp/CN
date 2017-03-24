#include <bits/stdc++.h>
#include "my_socket.h"
#include <sys/prctl.h>
using namespace std;
struct frame{
  int src_addr;
  int dstn_addr;
  int protocol;
};



int main(int argc, char* argv[]){
  if(argc < 4) {printf("usage %s <ip> <portno> <mac>\n",argv[0]); exit(0);}
  int sockfd = connect_server(argv[1],atoi(argv[2]));
  printf("sockfd = %d\n",sockfd);
  int mac = atoi(argv[3]);
  frame f;
  read(sockfd,(frame*)&f,sizeof(frame));
  if(f.protocol == -1){
    printf("All interface are in use....\n");
    exit(1);
  }
  else printf("Succesfully connected....!\n");



  int pid = fork();
  if(pid == 0){
    prctl(PR_SET_PDEATHSIG, SIGHUP);
    while(1){
      printf("enter destination MAC addr to send msg:"); fflush(stdout);
      int m;
      cin>>m;
      f.src_addr = mac;
      f.dstn_addr = m;
      write(sockfd,(char*)&f,sizeof(f));
    }
  }
  else{
    while(1){
      if( read(sockfd,(frame*)&f,sizeof(frame)) <= 0){
        printf("Problem in Switch....!!!\n");
        kill(pid,9);
        exit(1);
      }
      if(f.dstn_addr != mac) continue;
      printf("Msg from %d\n",f.src_addr);
    }
  }

  return 0;
}

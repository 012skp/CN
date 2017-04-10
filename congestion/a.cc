#include<bits/stdc++.h>
using namespace std;
#include<unistd.h>

double min(double a, int b){
  if(a<b)
  return a;
  else return (double)b;
}

#define rn rand()%100
void slow_start(double &cwnd, int &twnd, int &rwnd, int &thresh_hold){
  twnd = min(cwnd,rwnd);
  cwnd++;
}
void congestion_avoidance(double &cwnd, int &twnd, int &rwnd, int &thresh_hold){
  cwnd = cwnd + (double)1/(double)twnd;
  twnd = min(cwnd,rwnd);
}

double pd(double cwnd, int thresh_hold){
  if(cwnd < thresh_hold) return (double)rand()/(double)RAND_MAX;
  return (double)thresh_hold/cwnd;
}



int main(){
  cout<<"enter receving window size:";
  int rwnd,thresh_hold,twnd;
  double cwnd;
  cin>>rwnd;
  int rwnd_fix = rwnd;
  thresh_hold = rwnd;
  cwnd = 1;
  slow_start(cwnd,twnd,rwnd,thresh_hold);
  while(1){
    usleep(500000);
    printf("cwnd = %lf thresh_hold = %d\n",cwnd,thresh_hold);
    //timeout for 4% of packets...
    if(rn < 5*pd(cwnd,thresh_hold)){
      printf("timeout\n");
      thresh_hold = cwnd/2;
      cwnd = 1;
      slow_start(cwnd,twnd,rwnd,thresh_hold);
    }
    //3 dup acks for 1% of packets...
    else if(rn < 2*pd(cwnd,thresh_hold)){
      printf("3 dup ACK receievd\n");
      thresh_hold = cwnd/2;
      cwnd = thresh_hold;
      congestion_avoidance(cwnd,twnd,rwnd,thresh_hold);
    }
    else{// That is ACK received withing RTT....
      if(cwnd > thresh_hold) congestion_avoidance(cwnd,twnd,rwnd,thresh_hold);
      else slow_start(cwnd,twnd,rwnd,thresh_hold);
    }

  }
}

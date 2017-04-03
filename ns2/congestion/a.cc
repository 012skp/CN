#include<bits/stdc++.h>
using namespace std;
#define pb push_back



struct packet{
char event;
double time;
int from;
int to;
char packet_type[10];
int packet_size;
char flags[20];
int fid;
char src_addr[20];
char dstn_addr[20];
int seqno;
int packet_id;
};


bool comp(packet p1, packet p2){
	if(p1.packet_id < p2.packet_id ) return true;
	else if(p1.packet_id == p2.packet_id) return p1.event < p2.event;
	else return false;
}

int main(int argc, char* argv[]){
  if(argc < 2){printf("usage: %s <filename>\n",argv[0]); exit(1);}
char filename[100];
sprintf(filename,"%s",argv[1]);
	FILE *fp = fopen(filename,"r");
	vector<packet> packet_list;
	double throughput = 0;
	int recv=0,drop=0;
	while(!feof(fp)){
		packet p;
		fscanf(fp,"%c %lf %d %d %s %d %s %d %s %s %d %d\n", &p.event,&p.time,&p.from,&p.to,p.packet_type,&p.packet_size,p.flags,
									&p.fid,p.src_addr,p.dstn_addr,&p.seqno,&p.packet_id );
		packet_list.pb(p);
		//printf("%c %lf %d %d %s %d %s %d %s %s %d %d\n",p.event,p.time,p.from,p.to,p.packet_type,p.packet_size,p.flags,p.fid,
		//						p.src_addr,p.dstn_addr,p.seqno,p.packet_id);
		if(p.event == 'r'){ throughput+=p.packet_size; recv++;}
		if(p.event == 'd') drop++;
	}
	fclose(fp);
	double start,end;
	start = packet_list[0].time;
	end = packet_list[packet_list.size()-1].time;
	throughput = throughput/(end-start);
	printf("%lf\n",throughput);
return 0;
}

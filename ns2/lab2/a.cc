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
char filename[100];
int n = atoi(argv[1]);
FILE *fpp = fopen("output","w");
FILE *fppp = fopen("output1","w");
FILE *fpppp = fopen("output2","w");
for(int i=0;i<n;i++){
	sprintf(filename,"tracefile%d.tr",i);
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
	fprintf(fpp,"%lf %lf\n",(i+1)*0.5,throughput);

	//Delay Calculation..... Queing delay...
	sort(packet_list.begin(),packet_list.end(),comp);
	int ii = 0;
	int count = 0;
	double delay = 0;
	while(ii<packet_list.size()){
		int j = 0;
		while(packet_list[ii+j].packet_id == packet_list[ii].packet_id ) j++;
		if(packet_list[ii+1].event == '-'){
			delay += (packet_list[ii+1].time-packet_list[ii].time);
			count++;
		}
		ii+=j;
	}

	delay = delay/count;
	fprintf(fppp,"%lf %lf\n",(i+1)*0.5, delay); //output1
	fprintf(fpppp,"%lf %lf\n",(i+1)*0.5, (double)drop/(double)(drop+recv)); //output2

}
fclose(fpp);
fclose(fppp);
fclose(fpppp);

system("gnuplot -e \"set output '1.png'; set terminal png; plot 'output' with linespoint\" ");
system("gnuplot -e \"set output '2.png'; set terminal png; plot 'output1' with linespoint ,'output2' with linespoint\" ");
system("gnome-open 1.png");
return 0;
}

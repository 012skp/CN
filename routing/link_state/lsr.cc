#include<bits/stdc++.h>
using namespace std;
#include<unistd.h>

#define F first
#define S second
#define pb push_back
#define mp make_pair

typedef pair<int,int> pii;
typedef vector<set<pii> > graph;
typedef vector<pii> routing_table; // {via,cost}

struct LSP{
int node;
int seq;
set<pii> vec; //set of <node,cost>....!!!
};

class comp{
public:
	bool operator()(LSP p1, LSP p2){
		return p1.node < p2.node;
	}
};


void dijkastra_algo(vector<routing_table> &rt_table, graph &g, int node){	
	int n = g.size();
	vector<int> parent(n); parent[node] = node;
	vector<int> done(n,0); done[node] = 1;
	vector<int> cost(n,INT_MAX); cost[node] = 0;

	set<pii> s;
	s.insert(mp(0,node));
	for(int i=0;i<n;i++) if(i==node) continue; else s.insert({INT_MAX,i});
	
	while(!s.empty()){

		int from = s.begin()->S;
		done[from] = 1;
		s.erase(s.begin());

		for(auto a:g[from]){
			int to = a.F;
			if(done[to]) continue;
			int ccost = a.S;
			if(cost[to] > cost[from] + ccost){ // if prev cost > new cost through 'form' node then repalace...
				s.erase(s.find({cost[to],to}));
				cost[to] = cost[from]+ccost;
				s.insert({cost[to],to});
				parent[to] = from;
			}
		}

	}
	//Setting 'cost' and 'via' information in rt_table....
	for(int i=0;i<n;i++){
		int temp = i;
		while(parent[temp] != node) temp = parent[temp];
		rt_table[node][i].F = temp;
		rt_table[node][i].S = cost[i];
	}	
	
}



void print_rt_table(vector<routing_table> &rt_table, int node){
	printf("\nRouting Table of %d\n",node);
	printf("%-10s %-10s %-10s\n","TO","VIA","COST");
	for(int i=0;i<rt_table.size();i++){
		printf("%-10d %-10d %-10d\n",i,rt_table[node][i].F,rt_table[node][i].S);
	}
	cout<<endl;
}


int main(int argc, char* argv[]){
	if(argc < 2){printf("usage %s <input_filename>\n",argv[0]); exit(1);}
	FILE *fp = fopen(argv[1],"r");
	int n;
	fscanf(fp,"%d\n",&n);
	graph g(n);
	while(!feof(fp)){
		int u,v,w;
		u=v=w=0;
		fscanf(fp,"edge(%d,%d,%d)\n",&u,&v,&w);
		g[u].insert(mp(v,w));
		g[v].insert(mp(u,w));
		printf("edge(%d,%d,%d)\n",u,v,w);
	}
	
	vector<LSP> lsp_packets(n); // Initialise LSP packets for each node....
	for(int i=0;i<n;i++){
		lsp_packets[i].node = i;
		lsp_packets[i].seq = 1;
		for(auto p : g[i]){
			lsp_packets[i].vec.insert(p);
		}
	}
	
	vector<set<LSP,comp> > gn(n); // Each node will have a set of LSP....
	for(int i=0;i<n;i++) gn[i].insert(lsp_packets[i]); // Initialisation with their own LSP....
	
	//Start Flooding.....
	while(1){
		int flag = 0;
		printf("\n\nNext generation of Flooding....\n");
		for(int i=0;i<n;i++){
			//Node i is flooding to its neighbours....
			for(auto p : g[i]){ // For each neighbours 'p.F' of node 'i'
				int neighbour = p.F;
				pair<set<LSP>::iterator,bool> ret;
				for(auto packet : gn[i]){ // For each LSP packets that node i have, send it to neighbours
					if(packet.node == neighbour) continue;
					printf("Node %d  sending LSP of Node %d  to Node %d\n",i,packet.node,neighbour);
					ret = gn[neighbour].insert(packet); // If neighbour already has the packet with same node_id then LSP will be rejected...
					if(ret.S == false ){// If rejected check its seq_no.
					//If new seq_no of LSP is greater than previours one then replace it with newer LSP...
						if(lsp_packets[i].seq > ret.F->seq){
							gn[neighbour].erase(ret.F);
							gn[neighbour].insert(packet);
							flag = 1;
						}
					}
					else flag = 1; //Still new LSPs are being inserted... 
				}
			}
		}
		if(flag) continue;
		else break;
	}
	
	//Flooding over...
	
	//Route Computation...		
	vector<routing_table> rt_table(n,routing_table(n));
	for(int i=0;i<n;i++) dijkastra_algo(rt_table,g,i);
	for(int i=0;i<n;i++) print_rt_table(rt_table,i);
}

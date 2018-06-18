#include <vector>
#include <deque>
#include <map>
#include <tuple>
#include "util.h"
using namespace std;

// Connect pins in a single net 
void BFSRouting(vector<Pin> *netlist){
	//BFS for a single net
	// if (critical)
	// 	vector<vector<Pin> > *AllNetlist=criticalNets();
	// else vector<vector<Pin> > *AllNetlist=nonCriticalNets();
	// vector<Pin> *netlist=(*AllNetlist)[netnumber];
	
	int pinNum=netlist->size();
	int count=1;
	deque<vector<int> > q1;
	vector<vector<int> > u_adj; //adjacent vertices to u
	vector<int> u,adj;

	q1.push_back((*netlist)[0]);
	NetPoints.push_back((*netlist)[0]);
	for (int j=0;j<pinNum;j++)
		BFSDis[(*netlist)[j]]=-1;
	BFSDis[(*netlist)[0]]=0;

	while ((!q1.empty())&&(count<=(pinNum-1))){
		//dequeue vertex u
		u=q1.front(); 
		q1.pop_front();

		//discover adjacent points
		adj=u;
		adj[0]=u[0]-1; 
		u_adj.push_back(adj); //x-1 of u
		adj=u;
		adj[0]=u[0]+1; 
		u_adj.push_back(adj); //x+1 of u
		adj=u;
		adj[1]=u[1]-1; 
		u_adj.push_back(adj); //y-1 of u
		adj=u;
		adj[1]=u[1]+1; 
		u_adj.push_back(adj); //y+1 of u
		adj=u;
		adj[2]=u[2]-1; 
		if (u[2]!=1)
			u_adj.push_back(adj); //upper layer of u
		adj=u;
		adj[2]=u[2]+1; 
		if (u[2]!=4)
			u_adj.push_back(adj); //down layer of u
		
		//discovering each adjacnet points of u
		for (int i=0;i<u_adj.size();i++){ 
			if (!checkCross(u_adj[i][0],u_adj[i][1],u_adj[i][2])){
				//undiscovered points
				if (!checkExplored(u_adj[i])){  
					BFSDis[u_adj[i]]=BFSDis[u]+1;
					q1.push_back(u_adj[i]);   
				}
				//discover a pin
				else if (BFSDis[u_adj[i]]==-1){ 
					BFSDis[u_adj[i]]=BFSDis[u]+1;
					traceBFS(u_adj[i][0],u_adj[i][1],u_adj[i][2]);
					count++;
					q1.clear();
					BFSDis.clear();
					for (int j=0;j<pinNum;j++)
						BFSDis[(*netlist)[j]]=-1; 
					for (int k=0;k<NetPoints.size();k++){
							q1.push_back(NetPoints[k]);
							BFSDis[NetPoints[k]]=0;
					}
				}
			}
		}
		u_adj.clear();
	}
}

void HadlockRouting(int netnumber){
	//Hadlock plus BFS for a single net
	if (critical)
		vector<vector<Pin> > *AllNetlist=criticalNets();
	else vector<vector<Pin> > *AllNetlist=nonCriticalNets();
	vector<Pin> *netlist=(*AllNetlist)[netnumber];
	
	int pinNum=netlist->size();
	deque<vector<int> > q1,q2;
	vector<vector<int> > u_adj; //adjacent vertices to u
	vector<int> u,adj;
	
	deque<int> nonConnectPins;
	for (int i=1;i<netlist->size();i++)
		nonConnectPins.push_back(i);

	q1.push_back((*netlist)[0]);
	NetPoints.push_back((*netlist)[0]);
	for (int j=0;j<pinNum;j++)
		BFSDis[(*netlist)[j]]=-1;
	BFSDis[(*netlist)[0]]=0;

	//Hadlock Routing
	while(!nonConnectPins.empty()){
		int targetPin=nonConnectPins.front();
		bool findPin=false;
		int turn=10;
		
		while (findPin==false){
			//dequeue vertex u
			if (turn>0){
				u=q1.front(); 
				q1.pop_front();
				turn--;
			}
			else{
				u=q2.front(); 
				q2.pop_front();
				turn=10;
			}
			ManhattanDis[u]=ManhattanDist(u,(*netlist)[targetPin]);
			
			// adjacent points
			adj=u;
			adj[0]=u[0]-1; 
			u_adj.push_back(adj); //x-1 of u
			adj=u;
			adj[0]=u[0]+1; 
			u_adj.push_back(adj); //x+1 of u
			adj=u;
			adj[1]=u[1]-1; 
			u_adj.push_back(adj); //y-1 of u
			adj=u;
			adj[1]=u[1]+1; 
			u_adj.push_back(adj); //y+1 of u
			adj=u;
			adj[2]=u[2]-1; 
			if (u[2]!=1)
				u_adj.push_back(adj); //upper layer of u
			adj=u;
			adj[2]=u[2]+1; 
			if (u[2]!=4)
				u_adj.push_back(adj); //down layer of u
			
			//discovering each adjacnet points of u
			for (int i=0;i<u_adj.size();i++){ 
				if (!checkCross(u_adj[i][0],u_adj[i][1],u_adj[i][2])){
					//undiscovered points
					if (!checkExplored(u_adj[i])){  
						BFSDis[u_adj[i]]=BFSDis[u]+1;
						ManhattanDis[u_adj[i]]=ManhattanDist(u_adj[i],(*netlist)[targetPin]);
						
						if (ManhattanDist(u_adj[i],(*netlist)[targetPin])<ManhattanDist(u,(*netlist)[targetPin]))
							q1.push_back(u_adj[i]); 
						else q2.push_back(u_adj[i]);
					}
					//discover a pin
					else if (BFSDis[u_adj[i]]==-1){ 
						BFSDis[u_adj[i]]=BFSDis[u]+1;
						traceBFS(u_adj[i][0],u_adj[i][1],u_adj[i][2]);
						
						while((*netlist)[nonConnectPins.front()]!=u_adj[i]){
							nonConnectPins.push_back(nonConnectPins.front());
							nonConnectPins.pop_front();
						} 
						nonConnectPins.pop_front();
						q1.clear();
						q2.clear();
						BFSDis.clear();
						ManhattanDis.clear();
						for (int j=0;j<pinNum;j++)
							BFSDis[(*netlist)[j]]=-1; 
						for (int k=0;k<NetPoints.size();k++){
								q1.push_back(NetPoints[k]);
								BFSDis[NetPoints[k]]=0;
						}
					}
				}
			}
			u_adj.clear();
		}
	}

}





class Routing{
    public:
		// constructor
      Routing(){} 
		~Routing(){}

		void traceBFS(int x1, int y1, int z1){
			vector<vector<int> > path;//record points for output
			vector<int> u;
			path.push_back(u);
			path[path.size() - 1].push_back(x1);
			path[path.size() - 1].push_back(y1);
			path[path.size() - 1].push_back(z1);
		
		
			while(BFSDis[pinCoordinate(x1, y1, z1)] != 0)
			{
				if(z1 == 1 || z1 == 3)//only horizontal
				{
					if(BFSDis.find(pinCoordinate(x1 - 1, y1, z1)) != BFSDis.end() && (BFSDis[pinCoordinate(x1 - 1, y1, z1)] == BFSDis[pinCoordinate(x1, y1, z1)] - 1))//left
					{
						vector<int> v = pinCoordinate(x1, y1, z1);
						NetPoints.push_back(v);
						x1 = x1 - 1;
					}
					else if(BFSDis.find(pinCoordinate(x1 + 1, y1, z1)) != BFSDis.end() && (BFSDis[pinCoordinate(x1 + 1, y1, z1)] == BFSDis[pinCoordinate(x1, y1, z1)] - 1))//right 
					{
						vector<int> v = pinCoordinate(x1, y1, z1);
						NetPoints.push_back(v);
						x1 = x1 + 1;
					}
					else//go to other layers
					{
						if(x1 != path[path.size() - 1][0] && y1 != path[path.size() - 1][1])//only record the first jump
						{
							vector<int> u = pinCoordinate(x1, y1, z1);
							path.push_back(u);
						}
					
						if(z1 == 4)
						{
							z1 = 1;
						}
						else
						{
							z1 = z1 + 1;
						}
					}
				}
				else if(z1 == 2 || z1 == 4)//only vertical
				{
					if(BFSDis.find(pinCoordinate(x1, y1 - 1, z1)) != BFSDis.end() && (BFSDis[pinCoordinate(x1, y1 - 1, z1)] == BFSDis[pinCoordinate(x1, y1, z1)] - 1))//down 
					{
						vector<int> v;
						NetPoints.push_back(v);
						NetPoints[NetPoints.size() - 1].push_back(x1);
						NetPoints[NetPoints.size() - 1].push_back(y1);
						NetPoints[NetPoints.size() - 1].push_back(z1);
						y1 = y1 - 1;
					}
					else if(BFSDis.find(pinCoordinate(x1, y1 + 1, z1)) != BFSDis.end() && (BFSDis[pinCoordinate(x1, y1 + 1, z1)] == BFSDis[pinCoordinate(x1, y1, z1)] - 1))//up 
					{
						vector<int> v;
						NetPoints.push_back(v);
						NetPoints[NetPoints.size() - 1].push_back(x1);
						NetPoints[NetPoints.size() - 1].push_back(y1);
						NetPoints[NetPoints.size() - 1].push_back(z1);
						y1 = y1 + 1;
					}
					else//go to other layers
					{
						if(x1 != path[path.size() - 1][0] && y1 != path[path.size() - 1][1])//only record the first jump
						{
							vector<int> u;
							path.push_back(u);
							path[path.size() - 1].push_back(x1);
							path[path.size() - 1].push_back(y1);
							path[path.size() - 1].push_back(z1);
						}
					
						if(z1 == 4)
						{
							z1 = 1;
						}
						else
						{
							z1 = z1 + 1;
						}
					}
				}			
			}
			vector<int> u2;
			path.push_back(u2);
			path[path.size() - 1].push_back(x1);
			path[path.size() - 1].push_back(y1);
			path[path.size() - 1].push_back(z1);
			vector<int> v;
			NetPoints.push_back(v);
			NetPoints[NetPoints.size() - 1].push_back(x1);
			NetPoints[NetPoints.size() - 1].push_back(y1);
			NetPoints[NetPoints.size() - 1].push_back(z1);
		
			// add the net (start and end point) to netmap for future lookup check.
			for(int i = path.size()-1; i >= 1; i--)
			{
				addNet(path[i][2],path[i][0], path[i][1], path[i - 1][0], path[i - 1][1]);
			}
		
			//output path points(unfinished)
		
			path.clear();
		}

		
		bool checkExplored(vector<int> u){
			if (BFSDis.find(u)==BFSDis.end())
				return false;
			else return true;
		}
	
		int ManhattanDist(vector<int> x,vector<int> y){
			int distx,disty;
			if (x[0]>y[0])
				distx=(x[0]-y[0]);
			else distx=(y[0]-x[0]);
			
			if (x[1]>y[1])
				disty=(x[1]-y[1]);
			else disty=(y[1]-x[1]);
			
			return (distx+disty);
		}
		
		vector<int> pinCoordinate(int x,int y,int layer){
			vector<int> v1;
			v1.push_back(x);
			v1.push_back(y);
			v1.push_back(layer);
			return v1;
		}
		
	private:
		vector<vector<int> > NetPoints;	//(x,y,layer)
		map<vector<int>, int> BFSDis; //BFSDis[(x,y,layer)] = dis
		map<vector<int>, int> ManhattanDis;
};

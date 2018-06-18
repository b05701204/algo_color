#include "util.h"


using namespace std;



class Trace{
    public:
		// constructor
      Trace(){} 
		~Trace(){}

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
		
	private:
		vector<vector<int> > NetPoints;	//(x,y,layer)
		map<vector<int>, int> BFSDis; //BFSDis[(x,y,layer)] = dis
		map<vector<int>, int> ManhattanDis;
};
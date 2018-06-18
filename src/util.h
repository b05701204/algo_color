#include<map>
#include<vector>

using namespace std;

// (x,y,layer) coordinates
// Start x/y coordinate (x/y, layer)
typedef pair <int,int> Start;

typedef vector<int> Pin;
// CorMap stores the start coordinate of the net
typedef map <Start, vector<int>* > CorMap;

// NetMap stores the length of the net from the starting coordinate
typedef map <Pin,int > NetMap;

vector<int> pinCoordinate(int x,int y,int layer){
   vector<int> v1;
   v1.push_back(x);
   v1.push_back(y);
   v1.push_back(layer);
   return v1;
}


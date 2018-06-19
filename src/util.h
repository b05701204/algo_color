#include <map>
#include <vector>

using namespace std;
#ifndef UTIL
#define UTIL


// (x,y,layer) coordinates
// Start x/y coordinate (x/y, layer)
typedef pair<int, int> Start;

typedef vector<int> Pin;
// CorMap stores the start coordinate of the net
typedef map<Start, vector<int> *> CorMap;

// NetMap stores the length of the net from the starting coordinate
typedef map<Pin, int> NetMap;

vector<int> createPin(int x, int y, int layer)
{
    vector<int> v1;
    v1.push_back(x);
    v1.push_back(y);
    v1.push_back(layer);
    return v1;
}

int ManhattanDist(vector<int> x, vector<int> y)
{
    int distx, disty;
    if (x[0] > y[0])
        distx = (x[0] - y[0]);
    else
        distx = (y[0] - x[0]);
    if (x[1] > y[1])
        disty = (x[1] - y[1]);
    else
        disty = (y[1] - x[1]);
    return (distx + disty);
}
#endif
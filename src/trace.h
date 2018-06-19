#include<iostream>
#include "util.h"
#include "nets.h"
using namespace std;

#ifndef TRACE
#define TRACE

class Trace
{
  public:
    // constructor
    Trace(vector<Pin> *netlist) {
        int pinNum = netlist->size();
        for (int j = 0; j < pinNum; j++)
            disMap[(*netlist)[j]]= -1;
    }
    ~Trace() {}

    void traceBFS(int x1, int y1, int z1, Nets* nets,vector<vector<int> >* NetPoints)
    {
        vector<vector<int> > path; //record points for output
        path.push_back(createPin(x1, y1, z1));

        while (disMap[createPin(x1, y1, z1)] != 0)
        {
            if (z1 == 1 || z1 == 3) //only horizontal
            {
                if (disMap.find(createPin(x1 - 1, y1, z1)) != disMap.end() && (disMap[createPin(x1 - 1, y1, z1)] == disMap[createPin(x1, y1, z1)] - 1)) //left
                {
                    NetPoints->push_back(createPin(x1, y1, z1));
                    x1 = x1 - 1;
                }
                else if (disMap.find(createPin(x1 + 1, y1, z1)) != disMap.end() && (disMap[createPin(x1 + 1, y1, z1)] == disMap[createPin(x1, y1, z1)] - 1)) //right
                {
                    NetPoints->push_back(createPin(x1, y1, z1));
                    x1 = x1 + 1;
                }
                else //go to other layers
                {
                    if (x1 != path[path.size() - 1][0] && y1 != path[path.size() - 1][1]) //only record the first jump
                    {
                        path.push_back(createPin(x1, y1, z1));
                    }
                    if (z1 == 4)
                    {
                        z1 = 1;
                    }
                    else
                    {
                        z1 = z1 + 1;
                    }
                }
            }
            else if (z1 == 2 || z1 == 4) //only vertical
            {
                if (disMap.find(createPin(x1, y1 - 1, z1)) != disMap.end() && (disMap[createPin(x1, y1 - 1, z1)] == disMap[createPin(x1, y1, z1)] - 1)) //down
                {
                    NetPoints->push_back(createPin(x1,y1,z1));
                    y1 = y1 - 1;
                }
                else if (disMap.find(createPin(x1, y1 + 1, z1)) != disMap.end() && (disMap[createPin(x1, y1 + 1, z1)] == disMap[createPin(x1, y1, z1)] - 1)) //up
                {
                    NetPoints->push_back(createPin(x1,y1,z1));
                    y1 = y1 + 1;
                }
                else //go to other layers
                {
                    if (x1 != path[path.size() - 1][0] && y1 != path[path.size() - 1][1]) //only record the first jump
                    {
                        path.push_back(createPin(x1,y1,z1));
                    }

                    if (z1 == 4)
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
        path.push_back(createPin(x1,y1,z1));
        NetPoints->push_back(createPin(x1,y1,z1));
        // add the net (start and end point) to netmap for future lookup check.
        for (int i = path.size() - 1; i >= 1; i--)
        {
            cout<<"trace back"<<endl;
            nets->addNet(path[i][2], path[i][0], path[i][1], path[i - 1][0], path[i - 1][1]);
            cout<<"layer:"<<path[i][2]<<" points: "<<path[i][0]<<" "<<path[i][1]<<" "<< path[i - 1][0]<<" "<< path[i - 1][1]<<" "<<endl;
        }
    }

    bool checkExplored(vector<int> u)
    {
        if (disMap.find(u) == disMap.end())
            return false;
        else
            return true;
    }

    void addDis(vector<int> key, int value){
        disMap[key]=value;
    }

    int getDis(vector<int> key){
        return disMap[key];
    }

  private:
    map<vector<int>, int> disMap;  //disMap[(x,y,layer)] = dis
};

#endif
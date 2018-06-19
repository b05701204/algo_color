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
        //cout<<"IN"<<endl;
        vector<vector<int> > path; //record points for output
        path.push_back(createPin(x1, y1, z1));
        
        int count = 0;
        while (disMap[createPin(x1, y1, z1)] != 0)
        {
            //cout<<x1<<" "<<y1<<" "<<z1<<" "<<disMap[createPin(x1 , y1, z1)]<<endl;
            
            if (z1 == 1 || z1 == 3) //only horizontal
            {
                if (disMap.find(createPin(x1 - 1, y1, z1)) != disMap.end() && (disMap[createPin(x1 - 1, y1, z1)] == disMap[createPin(x1, y1, z1)] - 1)) //left
                {
                    if(count > 0)
                    {
                        path.push_back(createPin(x1,y1,z1));
                    }
                    NetPoints->push_back(createPin(x1, y1, z1));
                    x1 = x1 - 1;
                    count = 0;
                }
                else if (disMap.find(createPin(x1 + 1, y1, z1)) != disMap.end() && (disMap[createPin(x1 + 1, y1, z1)] == disMap[createPin(x1, y1, z1)] - 1)) //right
                {
                    if(count > 0)//just jump, path begin
                    {
                        path.push_back(createPin(x1,y1,z1));
                    }
                    NetPoints->push_back(createPin(x1, y1, z1));
                    x1 = x1 + 1;
                    count = 0;
                }
                else //go to other layers
                {
                    if(count == 0)//path end
                    {
                        path.push_back(createPin(x1,y1,z1));
                    }
                    count ++;
                    if(count >= 4)//break infinite loop
                    {
                        break;
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
                    if(count > 0)
                    {
                        path.push_back(createPin(x1,y1,z1));
                    }
                    NetPoints->push_back(createPin(x1,y1,z1));
                    y1 = y1 - 1;
                    count = 0;
                }
                else if (disMap.find(createPin(x1, y1 + 1, z1)) != disMap.end() && (disMap[createPin(x1, y1 + 1, z1)] == disMap[createPin(x1, y1, z1)] - 1)) //up
                {
                    if(count > 0)
                    {
                        path.push_back(createPin(x1,y1,z1));
                    }
                    NetPoints->push_back(createPin(x1,y1,z1));
                    y1 = y1 + 1;                 
                    count = 0;
                }
                else //go to other layers
                {
                    if(count == 0)//path end
                    {
                        path.push_back(createPin(x1,y1,z1));
                    }
                    count ++;
                    if(count >= 4)
                    {
                        break;
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
        if(int(path.size()) % 2 == 1)
        {
            path.push_back(createPin(x1,y1,z1));
        }
        
        NetPoints->push_back(createPin(x1,y1,z1));
        // add the net (start and end point) to netmap for future lookup check.
        //cout<<x1<<" "<<y1<<" "<<z1<<" "<<disMap[createPin(x1 , y1, z1)]<<endl;
        cout<<"trace back"<<endl;
        for (int i = path.size() - 1; i >= 1; i--)
        {
            nets->addOutput(path[i][2], path[i][0], path[i][1], path[i - 1][0], path[i - 1][1]);
            //nets->addNet(path[i][2], path[i][0], path[i][1], path[i - 1][0], path[i - 1][1]);
            //cout<<"layer:"<<path[i][2]<<" points: "<<path[i][0]<<" "<<path[i][1]<<" "<< path[i - 1][0]<<" "<< path[i - 1][1]<<" "<<endl;
            cout<<path[i][0]<<" "<<path[i][1]<<" "<<path[i][2]<<endl;
            cout<<path[i - 1][0]<<" "<<path[i - 1][1]<<" "<<path[i - 1][2]<<endl;
            i--;
        }
        cout<<endl;
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
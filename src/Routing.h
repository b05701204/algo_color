#include <vector>
#include <deque>
#include <map>
#include <tuple>
#include "util.h"
#include "nets.h"
#include "trace.h"
using namespace std;

// Connect pins in a single net
void BFSRouting(vector<Pin> *netlist, Nets *nets)
{

    // all the points have been visited
    vector<vector<int> > NetPoints;
    int pinNum = netlist->size();
    int count = 1;
    deque<vector<int> > Q;
    vector<vector<int> > u_adj; //adjacent vertices to u
    vector<int> u, adj;
    cout<<"first pin number"<<endl;
    cout<<(*netlist)[0][0]<<(*netlist)[0][1]<<(*netlist)[0][2]<<endl;
    Q.push_back((*netlist)[0]);
    NetPoints.push_back((*netlist)[0]);

    // trace to remember the distance matrix
    Trace trace(netlist);
    trace.addDis((*netlist)[0], 0);

    while ((!Q.empty()) && (count <= (pinNum - 1)))
    {
        //dequeue vertex u
        u = Q.front();
        Q.pop_front();

        // discover adjacent points
        adj = u;
        adj[0] = u[0] - 1;
        u_adj.push_back(adj); //x-1 of u
        adj = u;
        adj[0] = u[0] + 1;
        u_adj.push_back(adj); //x+1 of u
        adj = u;
        adj[1] = u[1] - 1;
        u_adj.push_back(adj); //y-1 of u
        adj = u;
        adj[1] = u[1] + 1;
        u_adj.push_back(adj); //y+1 of u
        adj = u;
        adj[2] = u[2] - 1;
        if (u[2] != 1)
            u_adj.push_back(adj); //upper layer of u
        adj = u;
        adj[2] = u[2] + 1;
        if (u[2] != 4)
            u_adj.push_back(adj); //down layer of u

        //discovering each adjacnet points of u
        for (int i = 0; i < u_adj.size(); i++)
        {
            if (!nets->checkCross(u_adj[i][0], u_adj[i][1], u_adj[i][2]))
            {
                // have discovered in the disMap
                if (trace.checkExplored(u_adj[i]))
                {
                    // turns out to be pin
                    if (trace.getDis(u_adj[i]) == -1)
                    {
                        trace.addDis(u_adj[i], trace.getDis(u) + 1);
                        trace.traceBFS(u_adj[i][0], u_adj[i][1], u_adj[i][2], nets, &NetPoints);
                        count++;
                        Q.clear();
                        trace = Trace(netlist);
                        for (int k = 0; k < NetPoints.size(); k++)
                        {
                            Q.push_back(NetPoints[k]);
                            trace.addDis(NetPoints[k], 0);
                        }
                    }
                }
                else
                { //undiscovered points
                    trace.addDis(u_adj[i], trace.getDis(u) + 1);
                    Q.push_back(u_adj[i]);
                }
            }
        }
        u_adj.clear();
    }
}

// void HadlockRouting(int netnumber)
// {
//     //Hadlock plus BFS for a single net
//     if (critical)
//         vector<vector<Pin>> *AllNetlist = criticalNets();
//     else
//         vector<vector<Pin>> *AllNetlist = nonCriticalNets();
//     vector<Pin> *netlist = (*AllNetlist)[netnumber];

//     int pinNum = netlist->size();
//     deque<vector<int>> Q, q2;
//     vector<vector<int>> u_adj; //adjacent vertices to u
//     vector<int> u, adj;

//     deque<int> nonConnectPins;
//     for (int i = 1; i < netlist->size(); i++)
//         nonConnectPins.push_back(i);

//     Q.push_back((*netlist)[0]);
//     NetPoints.push_back((*netlist)[0]);
//     for (int j = 0; j < pinNum; j++)
//         BFSDis[(*netlist)[j]] = -1;
//     BFSDis[(*netlist)[0]] = 0;

//     //Hadlock Routing
//     while (!nonConnectPins.empty())
//     {
//         int targetPin = nonConnectPins.front();
//         bool findPin = false;
//         int turn = 10;

//         while (findPin == false)
//         {
//             //dequeue vertex u
//             if (turn > 0)
//             {
//                 u = Q.front();
//                 Q.pop_front();
//                 turn--;
//             }
//             else
//             {
//                 u = q2.front();
//                 q2.pop_front();
//                 turn = 10;
//             }
//             ManhattanDis[u] = ManhattanDist(u, (*netlist)[targetPin]);

//             // adjacent points
//             adj = u;
//             adj[0] = u[0] - 1;
//             u_adj.push_back(adj); //x-1 of u
//             adj = u;
//             adj[0] = u[0] + 1;
//             u_adj.push_back(adj); //x+1 of u
//             adj = u;
//             adj[1] = u[1] - 1;
//             u_adj.push_back(adj); //y-1 of u
//             adj = u;
//             adj[1] = u[1] + 1;
//             u_adj.push_back(adj); //y+1 of u
//             adj = u;
//             adj[2] = u[2] - 1;
//             if (u[2] != 1)
//                 u_adj.push_back(adj); //upper layer of u
//             adj = u;
//             adj[2] = u[2] + 1;
//             if (u[2] != 4)
//                 u_adj.push_back(adj); //down layer of u

//             //discovering each adjacnet points of u
//             for (int i = 0; i < u_adj.size(); i++)
//             {
//                 if (!checkCross(u_adj[i][0], u_adj[i][1], u_adj[i][2]))
//                 {
//                     //undiscovered points
//                     if (!checkExplored(u_adj[i]))
//                     {
//                         BFSDis[u_adj[i]] = BFSDis[u] + 1;
//                         ManhattanDis[u_adj[i]] = ManhattanDist(u_adj[i], (*netlist)[targetPin]);

//                         if (ManhattanDist(u_adj[i], (*netlist)[targetPin]) < ManhattanDist(u, (*netlist)[targetPin]))
//                             Q.push_back(u_adj[i]);
//                         else
//                             q2.push_back(u_adj[i]);
//                     }
//                     //discover a pin
//                     else if (BFSDis[u_adj[i]] == -1)
//                     {
//                         BFSDis[u_adj[i]] = BFSDis[u] + 1;
//                         traceBFS(u_adj[i][0], u_adj[i][1], u_adj[i][2]);

//                         while ((*netlist)[nonConnectPins.front()] != u_adj[i])
//                         {
//                             nonConnectPins.push_back(nonConnectPins.front());
//                             nonConnectPins.pop_front();
//                         }
//                         nonConnectPins.pop_front();
//                         Q.clear();
//                         q2.clear();
//                         BFSDis.clear();
//                         ManhattanDis.clear();
//                         for (int j = 0; j < pinNum; j++)
//                             BFSDis[(*netlist)[j]] = -1;
//                         for (int k = 0; k < NetPoints.size(); k++)
//                         {
//                             Q.push_back(NetPoints[k]);
//                             BFSDis[NetPoints[k]] = 0;
//                         }
//                     }
//                 }
//             }
//             u_adj.clear();
//         }
//     }
// }

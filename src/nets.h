#include<fstream>
#include<ostream>
#include<map>
#include<vector>
#include<string>
#include<iostream>
#include<sstream>
#include<utility>
#include<algorithm>
#include"util.h"


using namespace std;


#ifndef NETS
#define NETS

class Nets {
    public:
    // constructor
    Nets(fstream* pin, fstream* net, fstream* block){
        readInFile(pin,net,block);
    }

    Nets(fstream* pin, fstream* net, fstream* block, fstream* out){
        readInFile(pin,net,block);
        output = out;
    }

    ~Nets(){
        for(CorMap::iterator it = x_map.begin(); it != x_map.end(); ++it) {
            delete it->second;
        }
        for(CorMap::iterator it = y_map.begin(); it != y_map.end(); ++it) {
            delete it->second;
        }
    }

    

    //check wether the net had crossed
    bool checkCross(int x,int y,int layer){
        // if cross, return true
        // check x_map first
        if (x_map.find(Start(x,layer)) != x_map.end()){
            vector<int>* y_values = x_map[Start(x,layer)];
            if (y_values->size() != 0){
                for (int i=y_values->size()-1;i>=0;i--){
                    int y_v = (*y_values)[i];
                    if( y >= y_v){
                        // check and exist
                        if(y <= (y_v+net_map[createPin(x,y_v,layer)])){
                            return true;
                        }
                        break;
                    }
                }
            }
        }
        // // check y_map
        if (y_map.find(Start(y,layer)) != y_map.end()){
            vector<int>* x_values = y_map[Start(y,layer)];
            if (x_values->size() != 0){
                for (int i=x_values->size()-1;i>=0;i--){
                    int x_v = (*x_values)[i];
                    if( x >= x_v){
                        // check and exist
                        if(x <= (x_v+net_map[createPin(x_v,y,layer)])){
                            return true;
                        }
                        break;
                    }
                }
            }
        }
        return false;
    }

    void addOutput(int layer,int x1,int y1,int x2,int y2){
        // default color
        int color=1;
        // coloring simple rule: if near one have net, change color
        addNet(layer,x1,y1,x2,y2,0);
        // output to file

    }


    vector<vector<Pin> >* criticalNets(){
        return &criNets;
    }

    vector<vector<Pin> >* nonCriticalNets(){
        return &noncriNets;
    }

    private:
    // data structure to store net and blockage
    CorMap x_map, y_map;
    NetMap net_map;
    NetMap color_map;

    fstream * output;

    // net to connect
    vector<vector<Pin > > criNets;
    vector<vector<Pin > > noncriNets;

    // the pins to connect
    vector<Pin > pins;

    // check near color, if exist, change to new color, if not return 0
    int checkNear(int layer,int x1,int y1,int x2,int y2,int color){
        // check the line parallel with x corr

    }

    // add the net (start and end point) to netmap for future lookup check.
    void addNet(int layer,int x1,int y1,int x2,int y2,int color){
        // add to lookup map and then add to length map
        if (x1 == x2){
            //add to x_map
            if(y1 < y2){
                //{x1,y1,layer};
                addMap(Start(x1,layer),y1,&x_map);
                net_map[createPin(x1,y1,layer)]=y2-y1;
                color_map[createPin(x1,y1,layer)]=color;
            }else{
                addMap(Start(x1,layer),y2,&x_map);
                net_map[createPin(x1,y2,layer)]=y1-y2;
                color_map[createPin(x1,y2,layer)]=color;
            }
        }else{
            //add to y_map
            if(x1 < x2){
                addMap(Start(y1,layer),x1,&y_map);
                net_map[createPin(x1,y1,layer)]= x2-x1;
                color_map[createPin(x1,y1,layer)]=color;
            }else{
                // int pin[]={x2,y1,layer};
                addMap(Start(y1,layer),x2,&y_map);
                net_map[createPin(x2,y1,layer)]= x1-x2;
                color_map[createPin(x2,y1,layer)]=color;
            }
        }
    }

    // add blockage to the map
    // (a,b) represent down-left vertex and (c,d) represent up-right vertex
    void addBlock(int a,int b,int c,int d){
        for (int i=0;i<4;i++){
            addNet(i,a,b,c,d,0);
            addNet(i,a,d,a,b,0);
            addNet(i,a,d,c,d,0);
            addNet(i,c,d,c,b,0);
        }
    }

    // convert double to int and *2 at the same time
    int doubleToInt(double num){
        num+=0.1;
        int intNum = (int) num*2;
        return intNum;
    }

    // add the starting corordinate and length to map
    /**************
    * x_map means the net is parallel with y axis
    * y_map means the net is parallel with x axis
    **********/
    void addMap(Start key, int value, CorMap* map){
        // can not find anything
        if (map->find(key) == map->end()){
            vector<int>* newValues = new vector<int>;
            newValues -> push_back(value);
            (*map)[key] = newValues;
        // add to the vector
        }else{
            (*map)[key]->push_back(value);
            vector<int>* values = (*map)[key];
            sort(values->begin(),values->end());
        }
    }

    void readInFile(fstream* pin, fstream* net, fstream* block){
        // read in pins
        int num,layer;
        double x, y;
        while( (*pin)>>num>>layer>>x>>y ){
            Pin onepin;
            //  = {doubleToInt(x),doubleToInt(y),layer};
            onepin.push_back(doubleToInt(x));
            onepin.push_back(doubleToInt(y));
            onepin.push_back(layer);
            cout<<"pin "<<num<<" :";
            cout<<onepin[0]<<onepin[1]<<onepin[2]<< endl;
            pins.push_back(onepin);
        }
        // read in blockage
        double x1,y1,x2,y2;
        while( (*block)>>x1>>y1>>x2>>y2 ){
            addBlock(
                doubleToInt(x1),doubleToInt(y1),doubleToInt(x2),doubleToInt(y2)
            );
        }
        // read in nets
        cout<<"net number"<<endl;
        string line;
        while (getline(*net, line)){
            vector<Pin> net;
            istringstream ss(line);
            int num;
            int i=0;
            while (ss >> num){
                if (i != 0){
                    cout<<num<<" : ";
                    cout<<pins[num-1][0]<<" "<<pins[num-1][1]<<" "<<pins[num-1][2]<<" "<<endl;
                    net.push_back(pins[num-1]);
                    
                }
                i++;
            }
            cout<<endl;
            if(line.back() == 'Y'){
                criNets.push_back(net);
            }else{
                noncriNets.push_back(net);
            }
        }
    }
};

#endif
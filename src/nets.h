#include<fstream>
#include<ostream>
#include<map>
#include<vector>
#include<string>
#include<sstream>
#include<utility>
#include<algorithm>
#include"util.h"


using namespace std;

class Connect {
    

    public:
    // constructor
    Connect(fstream* pin, fstream* net, fstream* block){
        readInFile(pin,net,block);
    }
    ~Connect(){
        for(CorMap::iterator it = x_map.begin(); it != x_map.end(); ++it) {
            delete it->second;
        }
        for(CorMap::iterator it = y_map.begin(); it != y_map.end(); ++it) {
            delete it->second;
        }
    }

    // add the net (start and end point) to netmap for future lookup check.
    void addNet(int layer,int x1,int y1,int x2,int y2){
        // add to lookup map and then add to length map
        Pin pin;
        if (x1 == x2){
            //add to x_map
            if(y1 < y2){
                pin.push_back(x1);
                pin.push_back(y1);
                pin.push_back(layer);
                //{x1,y1,layer};
                addMap(Start(x1,layer),y1,&x_map);
                net_map[pin]=y2-y1;
            }else{
                pin.push_back(x1);
                pin.push_back(y2);
                pin.push_back(layer);
                addMap(Start(x1,layer),y2,&x_map);
                net_map[pin]=y1-y2;
            }
        }else{
            //add to y_map
            if(x1 < x2){
                pin.push_back(x1);
                pin.push_back(y1);
                pin.push_back(layer);
                // int pin[]={x1,y1,layer};
                addMap(Start(y1,layer),x1,&y_map);
                net_map[pin]= x2-x1;
            }else{
                pin.push_back(x2);
                pin.push_back(y1);
                pin.push_back(layer);
                // int pin[]={x2,y1,layer};
                addMap(Start(y1,layer),x2,&y_map);
                net_map[pin]= x1-x2;
            }
        }
    }

    //check wether the net had crossed
    bool checkCross(int x,int y,int layer){
        // if cross, return true
        // check x_map first
        Pin pin;
        if (x_map.find(Start(x,layer)) != x_map.end()){
            vector<int>* y_values = x_map[Start(x,layer)];
            if (y_values->size() != 0){
                for (int i=y_values->size()-1;i>=0;i--){
                    int y_v = (*y_values)[i];
                    if( y >= y_v){
                        // check and exist
                        pin.push_back(x);
                        pin.push_back(y_v);
                        pin.push_back(layer);
                        // int pin[]={x,y_v,layer};
                        if(y <= (y_v+net_map[pin])){
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
                        pin.push_back(x_v);
                        pin.push_back(y);
                        pin.push_back(layer);
                        // int pin[]={x_v,y,layer};
                        if(x <= (x_v+net_map[pin])){
                            return true;
                        }
                        break;
                    }
                }
            }
        }
        return false;
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

    // net to connect
    vector<vector<Pin > > criNets;
    vector<vector<Pin > > noncriNets;

    // the pins to connect
    vector<Pin > pins;


    // add blockage to the map
    // (a,b) represent down-left vertex and (c,d) represent up-right vertex
    void addBlock(int a,int b,int c,int d){
        for (int i=0;i<4;i++){
            addNet(i,a,b,c,d);
            addNet(i,a,d,a,b);
            addNet(i,a,d,c,d);
            addNet(i,c,d,c,b);
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
        string line;
        while (getline(*net, line)){
            vector<Pin> net;
            istringstream ss(line);
            int num;
            int i=0;
            while (ss >> num){
                if (i != 0){
                    net.push_back(pins[num]);
                }
            }
            if(line.back() == 'Y'){
                criNets.push_back(net);
            }else{
                noncriNets.push_back(net);
            }
        }
    }
};
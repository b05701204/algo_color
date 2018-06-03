/*****************
 * This is the main program of Color routing
 * ***************/
#include<fstream>
#include<ostream>
#include<map>
#include<vector>
#include<string>
#include<iomanip>
#include<utility>
#include<sstream>
using namespace std;

// (x,y) coordinates
typedef pair<int,int> Key;
// CorMap stores the start coordinate of the net
typedef map <int, vector<int>* > CorMap;
// NetMap stores the length of the net from the starting coordinate
typedef map<Key, int > NetMap;
// store the current trace points
typedef vector<vector<int> > NetPoints;

class Connect {
    // data structure to store net and blockage
    CorMap x_map, y_map;
    NetMap net_map;

    // net to connect
    vector<vector<int> > nets;
    vector<char> net_critical;

    // the pins to connect
    vector<Key> pins;
    vector<int> pinLayer;

    public:

    // constructor
    Connect(fstream* pin, fstream* net, fstream* block){
        readInFile(pin,net,block);
    }

    // add the net (start and end point) to netmap for future lookup check.
    void addNet(int layer,int x1,int y1,int x2,int y2){
        // add to lookup map and then add to length map
        if (x1 == x2){
            //add to x_map
            if(y1 < y2){
                addMap(x1,y1,&x_map);
                net_map[Key(x1,y1)]=y2-y1;
            }else{
                addMap(x1,y2,&x_map);
                net_map[Key(x1,y2)]=y1-y2;
            }
        }else{
            //add to y_map
            if(x1 < x2){
                addMap(y1,x1,&y_map);
                net_map[Key(x1,y1)]= x2-x1;
            }else{
                addMap(y1,x2,&y_map);
                net_map[Key(x2,y1)]= x1-x2;
            }
        }
    }

    //check wether the net had crossed
    bool checkCross(int x,int y){
        // if cross, return true
        // check x_map first
        if (x_map.find(x) != x_map.end()){
            vector<int>* y_values = x_map[x];
            if (y_values->size() != 0){
                for (int i=y_values->size()-1;i>=0;i--){
                    int y_v = (*y_values)[i];
                    if( y >= y_v){
                        // check and exist
                        if(y <= (y_v+net_map[Key(x,y_v)])){
                            return true;
                        }
                        break;
                    }
                }
            }
        }
        // // check y_map
        if (y_map.find(y) != y_map.end()){
            vector<int>* x_values = y_map[y];
            if (x_values->size() != 0){
                for (int i=x_values->size()-1;i>=0;i--){
                    int x_v = (*x_values)[i];
                    if( x >= x_v){
                        // check and exist
                        if(x <= (x_v+net_map[Key(x_v,y)])){
                            return true;
                        }
                        break;
                    }
                }
            }
        }
        return false;
    }

    private:

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
    void addMap(int key,int value,CorMap* map){
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
            pinLayer.push_back(layer);
            pins.push_back(Key(doubleToInt(x),doubleToInt(y)));
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
            vector<int> net;
            istringstream ss(line);
            double num;
            int i=0;
            while (ss >> num){
                if (i != 0){
                    net.push_back(doubleToInt(num));
                }
            }
            net_critical.push_back(line.back());
            nets.push_back(net);
        }
    }


};

int main (int argc, char* argv[]){
    // read in the parameters
    fstream pin,net,blockage;
    pin.open(argv[1],fstream::in);
    net.open(argv[2],fstream::in);
    blockage.open(argv[3],fstream::in);

    fstream out;
    out.open(argv[4],fstream::out);

    // initialize the connecting class
    Connect connect(&pin,&net,&blockage);

}
/*****************
 * 2018 6,7 20:07
 * This is the main program of Color routing
 * ***************/
#include<fstream>
#include<ostream>
#include<iostream>
#include "nets.h"
#include "Routing.h"

using namespace std;

  // Perform routing for all netlists
void NetRouting(Connect *connect){
	//routing for critical nets
	vector<vector<Pin> >* crticalNets = connect->criticalNets();
	for (int i=0;i<crticalNets->size();i++){
		BFSRouting(&(*crticalNets)[i]);
	}
	//routing for non-critical nets
	vector<vector<Pin> >* nonCriticalNets=connect->nonCriticalNets();
	for (int j=0;j<nonCriticalNets()->size();j++){
		BFSRouting(&(*nonCriticalNets)[i]);
	}
}


int main (int argc, char* argv[]){
    // read in the parameters
    fstream pin,net,blockage;
    pin.open(argv[1],fstream::in);
    net.open(argv[2],fstream::in);
    blockage.open(argv[3],fstream::in);

    // fstream out;
    // out.open(argv[4],fstream::out);

    // initialize the connecting class
    Connect connect(&pin,&net,&blockage);

}
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
void NetRouting(Nets* nets){
	//routing for critical nets
	vector<vector<Pin> >* crticalNets = nets->criticalNets();
	for (int i=0;i<crticalNets->size();i++){
		BFSRouting(&(*crticalNets)[i],nets);
	}
	//routing for non-critical nets
	vector<vector<Pin> >* nonCriticalNets=nets->nonCriticalNets();
	for (int j=0;j<nonCriticalNets->size();j++){
		BFSRouting(&(*nonCriticalNets)[j],nets);
	}
}


int main (int argc, char* argv[]){
    // read in the parameters
    fstream pin,net,blockage;
    pin.open(argv[1],fstream::in);
    net.open(argv[2],fstream::in);
    blockage.open(argv[3],fstream::in);

    fstream out;
    out.open(argv[4],fstream::out);

    // initialize the connecting class
    Nets nets(&pin,&net,&blockage,&out);

    // start routing
    NetRouting(&nets);


}
/*
  Project: OpenCogER
  File: exUDPserver.cpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: 2017
*/

#include "network/StringServerUDP.hpp"
#include <vector>
#include <string>
#include <iostream>

#include "time.h"

using namespace std;
#define PORTN 5585
int main(void)
{
    vector<string> tgs;
    tgs.push_back("stt");
    string msg,hst;
    
    StringServerUDP ss(tgs,PORTN);
    if (!ss.isOk()){
        cout<<"error opening socket";
        return -1;
    }
    cout<<"starting server at port.."<<PORTN<<endl;
    while(true){
        usleep(100*1000);
        msg = ss.readCurrentString("stt",hst);
        if (msg.length()>0){
            cout<<endl<<"host: "<<hst<<endl;
            cout<<"msg: "<<msg<<endl;
        }
    }
    return 0;
}
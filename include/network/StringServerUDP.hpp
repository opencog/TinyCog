/*
  Project: OpenCogER
  File: StringServerUDP.hpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: 2017
*/
#ifndef STRINGSERVERUDP_H
#define STRINGSERVERUDP_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <thread>
#include <mutex>

using namespace std;

class StringServerUDP
{
public:
    StringServerUDP(vector<string>tag_names,int portno);
    ~StringServerUDP();
    bool isOk();
    string readCurrentString(string tag);

protected:
    bool ok;
    map<string,string> msg_map;
    vector<string> tags;
    thread* th;
    mutex mtx;
    
    int sockfd; /* socket */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr; /* client addr */
    socklen_t clientlen; /* byte size of client's address */
    struct hostent *hostp; /* client host info */

    //char *hostaddrp; /* dotted decimal host addr string */
    //int optval; /* flag value for setsockopt */
    //int n; /* message byte size */

private:
    bool addTag(string tag);
    static void threadLoop(StringServerUDP* ss);
};
#endif
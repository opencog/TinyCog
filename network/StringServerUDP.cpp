/*
  Project: OpenCogER
  File: StringServerUDP.cpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: 2017
*/
#include "network/StringServerUDP.hpp"
#define BUFSIZE 1024

StringServerUDP::StringServerUDP(vector<string>tag_names,int portno)
{
  //portno; /* port to listen on */
  
  ok=false;
  for (int i=0;i<tag_names.size();i++)
  {
      addTag(tag_names[i]);
  }
  if (tags.size()<1)return;
  /* 
  * socket: create the parent socket 
  */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0){
      cout<<"\nError opening socket"<<endl;
      return;
  } 
   /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  int optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));
  /*set 1 sec time out*/
  struct timeval read_timeout;
  read_timeout.tv_sec = 1;
  read_timeout.tv_usec = 0;
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, 
      &read_timeout, sizeof(read_timeout));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  /* 
   * bind: associate the parent socket with a port 
   */
  if (bind(sockfd, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0){ 
    cout<<"\nERROR on binding"<<endl;
    return;
  }
  clientlen = sizeof(clientaddr);

  ok=true;
  th = new thread(threadLoop,this);
}

StringServerUDP::~StringServerUDP()
{
    ok=false;
    if (th){
        th->join();
        delete th;
    }
}

bool StringServerUDP::isOk()
{
    return ok;
}

bool StringServerUDP::addTag(string tag)
{
    if (tag.length()<1) return false;
    if (tags.size()<1) {
        tags.push_back(tag);
        return true;
    }
    if (find(tags.begin(),tags.end(),tag)!=tags.end()){
        return false;
    }
    tags.push_back(tag);
    return true;
}

string StringServerUDP::readCurrentString(string tag,string& host_addr)
{
    string result;
    if (find(tags.begin(),tags.end(),tag)==tags.end())
        return result;
    mtx.lock();
    result = msg_map[tag];
    msg_map[tag] = "";
    host_addr = host_map[tag];
    host_map[tag] = "";
    mtx.unlock();
    return result;
}

void StringServerUDP::threadLoop(StringServerUDP* ss)
{
    char buf[BUFSIZE]; /* message buf */
    struct hostent *hostp; /* client host info */
    char *hostaddrp; /* dotted decimal host addr string */
    string tmp,tg,st,host_addr;
    size_t pos;
    int n;
    while (ss->ok){
      /*
       * recvfrom: receive a UDP datagram from a client
      */
      bzero(buf, BUFSIZE);
      n = recvfrom(ss->sockfd, buf, BUFSIZE, 0,
		 (struct sockaddr *) &(ss->clientaddr), &(ss->clientlen));
      if (n < 0)
        cout<<"no read in recvfrom"<<endl;
      if (n >2 && n < BUFSIZE){
          if (buf[n-1]!='\0')buf[n]='\0';
          //process buf
          if (buf[0]!='<') continue;
          tmp=buf;
          pos=tmp.find_first_of('>',2);
          if (pos==string::npos)continue;
          //look for tag
          tg = tmp.substr(0,pos-1);
          if (find(ss->tags.begin(),ss->tags.end(),tg)==ss->tags.end())continue;
          /* 
           * gethostbyaddr: determine who sent the datagram
          */
          hostp = gethostbyaddr((const char *)&(ss->clientaddr.sin_addr.s_addr), 
			  sizeof(ss->clientaddr.sin_addr.s_addr), AF_INET);
          if (hostp == NULL)
              cout<<"ERROR on gethostbyaddr"<<endl;
          hostaddrp = inet_ntoa(ss->clientaddr.sin_addr);
          host_addr="";
          if (hostaddrp == NULL)
             cout<<"ERROR on inet_ntoa\n";
          else
              host_addr = hostaddrp;
             
          if (pos<tmp.length()-1){
              st = tmp.substr(pos,tmp.length()-pos-1);
              ss->mtx.lock();
              ss->msg_map[tg]=st;
              ss->host_map[tg]=host_addr;
              ss->mtx.unlock();
          }
      }
    }
}
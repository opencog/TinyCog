/*
  Project: OpenCogER
  File: TTS.hpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: 2017
*/
#ifndef TTS_H
#define TTS_H
#include <string>
using namespace std;

class TTS
{
    public:
    TTS(){};
    virtual ~TTS(){};
    virtual bool isOk()=0;
    virtual string getMessage()=0;
    virtual bool speak(string spk)=0;
    
    protected:
    bool ok;
    string msg;
};
#endif
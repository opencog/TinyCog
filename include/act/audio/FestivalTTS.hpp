/*
  Project: OpenCogER
  File: FstivalTTS.hpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: 2017
*/
#ifndef FESTIVALTTS_H
#define FESTIVALTTS_H

#include "TTS.hpp"
#include "festival.h"

class FestivalTTS: public TTS
{
    public:
    enum speaker{RAB=0,DON,KAL};
    FestivalTTS();
    virtual ~FestivalTTS();
    void setSpeaker(speaker spkr);
    bool isOk();
    string getMessage();
    bool speak(string spk);
};
#endif
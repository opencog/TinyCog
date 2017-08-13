/*
  Project: OpenCogER
  File: FestivalTTS.cpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: 2017
*/

#include "act/audio/FestivalTTS.hpp"

FestivalTTS::FestivalTTS():TTS()
{
    //init
    ok=true;
    msg="";
    int heap_size = 210000;  // default scheme heap size
    int load_init_files = 1; // we want the festival init files loaded

    festival_initialize(load_init_files,heap_size);
}

FestivalTTS::~FestivalTTS()
{
    festival_wait_for_spooler();
}

bool FestivalTTS::isOk()
{
    return ok;
}

string FestivalTTS::getMessage()
{
    return msg;
}

bool FestivalTTS::speak(string spk)
{
    if (festival_say_text(spk.c_str()))return true;
    return false;
}

void FestivalTTS::setSpeaker(speaker spkr)
{
    string st;
    switch(spkr)
    {
        case speaker::RAB:
        st="(voice_rab_diphone)";
        break;
        case speaker::DON:
        st="(voice_don_diphone)";
        break;
        case speaker::KAL:
        st="(voice_kal_diphone)";
        break;
        default:
        st="(voice_kal_diphone)";
    }
    festival_eval_command(st.c_str());
}
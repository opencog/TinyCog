/*
  Project: OpenCogER
  File: exampleTTS.cpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: 2017
*/

#include "act/audio/FestivalTTS.hpp"

int main(int argc, char** argv)
{
    FestivalTTS tts;
    tts.setSpeaker(FestivalTTS::speaker::DON);
    if (argc>1){
        tts.speak(argv[1]);
    } else {
        tts.speak("I have nothing to say");
    }
    return 0;
}
/*
 * File: STT_Test.cpp
 * Author: Dagim Sisay <dagiopia@gmail.com>
 * License: AGPL
 * Date: May, 2018
*/

#include <stdio.h>
#include <signal.h>

#include "sense/audio/SSTT.hpp"

void sigint_handler(int sig)
{
    printf("Caught SIGINT! Exiting...\n");
    tcstt_close();
}

void stt_callback(const char *text)
{
    printf("Human Said: %s\n", text);
}

int main(int argc, char **argv)
{
    signal(SIGINT, sigint_handler);
    tcstt_init(stt_callback);
    tcstt_close();
    while(tcstt_is_on()) ;
return 0;
}

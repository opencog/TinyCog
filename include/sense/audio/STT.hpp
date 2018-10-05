/*
 * File: SSTT.hpp
 * Author: Dagim Sisay <dagiopia@gmail.com>
 * License: AGPL 
 * Date: May, 2018
*/

#ifndef _SSTT_HPP_
#define _SSTT_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string>
#include <thread>
#include <mutex>

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <pocketsphinx.h>

#include "sense/audio/AudioCap.hpp"

#define SAMPLE_RATE 16000

//Functions
void tcstt_init (void (*f)(const char *s));
void tcstt_close ();
bool tcstt_is_on();
void tcstt_run();


#endif //_SSTT_HPP_

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
void init_ps();
void init_ad();
void init_ac();
void init_socket(std::string, bool client);

void ac_callback(void *buffer, uint32_t size);
void socket_send(const char *s);

void tcstt_init (bool ps_ad, void (*f)(const char *s));
void tcstt_init (bool ps_ad, std::string address, bool client);
void tcstt_close ();
bool tcstt_is_on();
void tcstt_run();


#endif //_SSTT_HPP_

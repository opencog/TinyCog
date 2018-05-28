/*
 * File: SSTT.hpp
 * Author: Dagim Sisay <dagiopia@gmail.com>
 * License: AGPL 
 * Date: May, 2018
*/

#ifndef _SSTT_HPP_
#define _SSTT_HPP_

#include <stdio.h>
#include <string>
#include <unistd.h>

#include <sphinxbase/err.h>
#include <sphinxbase/err.h>
#include <pocketsphinx.h>

#define SAMPLE_RATE 16000

class SSTT {
    public:
      SSTT();
      ~SSTT () {}
      bool is_on();
      void spx_stt(void (*)());

    private: 
      ps_decoder_t *ps;
      cmd_ln_t *cfg;
      ad_rec_t *ad;
      int16 adbuf[2048];
      uint8 utt_started, in_speech;
      int32 k;
      char const *hyp;
};

#endif //_SSTT_HPP_

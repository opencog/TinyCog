
#ifndef _AUDIO_CAP_HPP_
#define _AUDIO_CAP_HPP_

// newer alsa api
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
#include <stdint.h>
#include <string>

using namespace std;

class AudioCap {
    
    public:
      //constructor for reading on demand
      AudioCap(string dev="default", uint16_t sample_rate=16000, 
               snd_pcm_format_t pcm_format=SND_PCM_FORMAT_S16_LE);
      //Constructor for with callback function
      AudioCap(string dev="default", uint16_t sample_rate=16000, 
		         snd_pcm_format_t pcm_format=SND_PCM_FORMAT_S16_LE,
               void(*f) (void *a));

      bool is_OK;
		char *get_err();
		void *read();

    private:
      snd_pcm_t *d_handle;
      snd_pcm_hw_params_t *hw_params;
		snd_pcm_sw_params_t *sw_params;
      snd_pcm_uframes_t frms;
      uint32_t size, loops;
      void *buffer;
      uint16_t val, ret;
		int dir;
		void (*func)(uint16_t *buf);

};



#endif // _AUDIO_CAP_HPP_

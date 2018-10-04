
#ifndef _AUDIO_CAP_HPP_
#define _AUDIO_CAP_HPP_

// newer alsa api
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
#include <stdint.h>
#include <string>
#include <thread>
#include <mutex>

using namespace std;

class AudioCap {
    
public:
	//Constructor for with callback function
	AudioCap(string dev="default", unsigned int sample_rate=16000, 
	         snd_pcm_format_t pcm_format=SND_PCM_FORMAT_S16_LE,
	         void(*f) (void *, uint32_t )=NULL);

	~AudioCap();

	bool is_OK() { return is_running; }

private:
	bool is_running;
	static void run_capture(AudioCap*);
	
	thread *run;

	snd_pcm_t *d_handle;
	snd_pcm_hw_params_t *hw_params;
	snd_pcm_uframes_t frms;
	uint32_t size, loops;
	void *buffer;
	uint16_t val, ret;
	int dir;
	void (*cb_func)(void *buf, uint32_t size);
	

};



#endif // _AUDIO_CAP_HPP_


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

/*
 * Only audio with a width of 16 bits and sample rate at 16000
 * because the main goal is to use in stt and pocketsphinx  models
 * mostly prefer it. 
*/


public:
	AudioCap(string dev="default");
	void set_callback(void(*f) (void*, uint32_t));

	~AudioCap() { shutdown(); }

	bool is_OK() { return is_running; }
	void start();
	void pause();
	void shutdown();

private:
	void setup();
	bool is_running;
	static void run_capture(AudioCap*);
	thread *run;

	string device;
	unsigned int sample_rate;
	snd_pcm_format_t pcm_format;
	snd_pcm_t *d_handle;
	snd_pcm_hw_params_t *hw_params;
	snd_pcm_uframes_t frms;
	uint32_t size;
	void *buffer;
	uint16_t val, ret;
	int dir;
	void (*cb_func)(void *buf, uint32_t samples);
};



#endif // _AUDIO_CAP_HPP_

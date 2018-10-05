
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
	// s/u , 8/16/32 , LE/BE
	snd_pcm_format_t AUDIO_FMT[2][3][2] = {{{SND_PCM_FORMAT_S8, SND_PCM_FORMAT_S8},
	                                  {SND_PCM_FORMAT_S16_LE, SND_PCM_FORMAT_S16_BE},
	                                  {SND_PCM_FORMAT_S32_LE, SND_PCM_FORMAT_S32_BE}},
                                    {{SND_PCM_FORMAT_U8, SND_PCM_FORMAT_U8},
                                     {SND_PCM_FORMAT_U16_LE, SND_PCM_FORMAT_U16_BE},
                                     {SND_PCM_FORMAT_U32_LE, SND_PCM_FORMAT_U32_BE}}}; 
	enum fmt_bit_width {BYTE=0, WORD, DWORD}; // 8, 16, AND 32 bits

	//Constructor for with callback function
	AudioCap(string dev="default", 
	         unsigned int sample_rate=16000, 
	         bool signd=true, 
				fmt_bit_width bit_width=fmt_bit_width::WORD,
				bool little_endian=true);
	void set_callback(void(*f) (void*, uint32_t));

	~AudioCap();

	bool is_OK() { return is_running; }
	void start();
	void stop();

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
	uint32_t size, loops;
	void *buffer;
	uint16_t val, ret;
	int dir;
	void (*cb_func)(void *buf, uint32_t size);
};



#endif // _AUDIO_CAP_HPP_

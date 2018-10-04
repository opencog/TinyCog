
#include "sense/audio/AudioCap.hpp"

AudioCap::AudioCap(string dev, unsigned int sample_rate, 
                   snd_pcm_format_t pcm_format, 
                   void(*f)(void *b, uint32_t s))
{
	 is_running = false;
	 frms = 16;
	 //open device
	 ret = snd_pcm_open(&d_handle, dev.c_str(), SND_PCM_STREAM_CAPTURE, 0);
	 if(ret < 0) {
	   fprintf(stderr, "ERROR: Unable to open PCM Device for Recording: %s\n",
		                snd_strerror(ret));
		exit(1);
	}

	snd_pcm_hw_params_alloca(&hw_params);

	snd_pcm_hw_params_any(d_handle, hw_params);

	snd_pcm_hw_params_set_format(d_handle, hw_params, pcm_format); // set format
	snd_pcm_hw_params_set_channels(d_handle, hw_params, 1); // set channel
	snd_pcm_hw_params_set_rate_near(d_handle, hw_params, &sample_rate, &dir); //set sample rate
	snd_pcm_hw_params_set_period_size_near(d_handle, hw_params, &frms, &dir); //set period size

	ret = snd_pcm_hw_params(d_handle, hw_params);
	if(ret < 0) {
       fprintf(stderr, "Unable to set HW Params: %s\n", snd_strerror(ret));
		 exit(1);
	}

	snd_pcm_hw_params_get_period_size(hw_params, &frms, &dir);
	size = frms * 2;
	buffer = malloc(size); //XXX typed buffer could be good.... don't know

	snd_pcm_hw_params_get_period_time(hw_params, &sample_rate, &dir);

	fprintf(stdout, "Dir = %d\n", dir);
	fprintf(stdout, "Sample Rate = %d\n", sample_rate);
   
	cb_func = f;

	is_running = true;

	run = new thread(AudioCap::run_capture, this);
}


AudioCap::~AudioCap()
{
	is_running = false;
	run->join();
	snd_pcm_drain(d_handle);
	snd_pcm_close(d_handle);
	free(buffer);
	printf("CLOSED AUDIO CAPTURE DOWN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
}


void AudioCap::run_capture(AudioCap *ac)
{
	int r;
	while(ac->is_running) {
		r = snd_pcm_readi(ac->d_handle, ac->buffer, ac->frms);
		if(r == -EPIPE) {
			fprintf(stderr, "Warning: Overrun!\n");
			snd_pcm_prepare(ac->d_handle);
		}
		else if(r < 0) {
			fprintf(stderr, "Error: Problem Reading: %s\n", snd_strerror(r));
		}
		else if(r != (int)ac->frms) {
			fprintf(stderr, "Warning: Short Read %d frames\n", r);
		}

		//call callback
		ac->cb_func(ac->buffer, ac->size);
	}
}


#include "AudioCap.hpp"

AudioCap::AudioCap(string dev="default", uint16_t sample_rate=16000, 
                   snd_pcm_format_t pcm_format=SND_PCM_FORMAT_S16_LE)
{
    is_OK = false;
	 frms = 16;
	 //open device
	 ret = snd_pcm_open(&d_handle, dev, pcm_format, 0);
	 if(ret < 0) {
	   fprintf(stderr, "ERROR: Unable to open PCM Device for Recording: %s\n",
		                snd_strerror(ret));
		exit(1);
	}

	snd_pcm_hw_params_alloca(&hw_params);
	snd_pcm_sw_params_alloca(&sw_params);

	snd_pcm_hw_params_set_format(d_handle, hw_params, pcm_format); // set format
	snd_pcm_hw_params_set_channels(d_handle, hw_params, 1); // set channel
	snd_pcm_hw_params_set_rate_near(d_handle, hw_params, &sample_rate, &dir); //set sample rate
	snd_pcm_hw_params_set_period_size_near(d_handle, hw_params, &frms, &dir); //set period size

	ret = snd_pcm_params_(d_handle, hw_params);
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

   is_OK = true;

	while (true) {
	    ret = snd_pcm_readi(d_handle, buffer, frms);
		 if(ret == -EPIPE) {
		     fprintf(stderr, "Warning: Overrun!\n");
			  snd_pcm_prepare(d_handle);
		 }
		 else if(ret < 0) {
		     fprintf(stderr, "Error: Problem Reading: %s\n", snd_strerror(ret));
		 }
		 else if(ret != (int)frms) {
		     fprintf(stderr, "Warning: Short Read %d frames\n", ret);
		 }
	}

	snd_pcm_drain(cap_handle);
	snd_pcm_close(cap_handle);
	free(buffer);
}




AudioCap::AudioCap(string dev="default", uint16_t sample_rate=16000, 
                   snd_pcm_format_t pcm_format=SND_PCM_FORMAT_S16_LE, void(*f)(void *a))
{
    is_OK = false;
	 frms = 16;
	 //open device
	 ret = snd_pcm_open(&d_handle, dev, pcm_format, 0);
	 if(ret < 0) {
	   fprintf(stderr, "ERROR: Unable to open PCM Device for Recording: %s\n",
		                snd_strerror(ret));
		exit(1);
	}

	snd_pcm_hw_params_alloca(&hw_params);
	snd_pcm_sw_params_alloca(&sw_params);

	snd_pcm_hw_params_set_format(d_handle, hw_params, pcm_format); // set format
	snd_pcm_hw_params_set_channels(d_handle, hw_params, 1); // set channel
	snd_pcm_hw_params_set_rate_near(d_handle, hw_params, &sample_rate, &dir); //set sample rate
	snd_pcm_hw_params_set_period_size_near(d_handle, hw_params, &frms, &dir); //set period size

	ret = snd_pcm_params_(d_handle, hw_params);
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
   
	is_OK = true;

}

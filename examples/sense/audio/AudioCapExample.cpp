/*
	File: AudioCapExample.cpp

	run: ./AudioCapExample | aplay -c 1 -r 16000 -f S16_LE  # for live playback
	     or 
		  ./AudioCapExample > my_rec.raw  # for recording to file
*/

#include "sense/audio/AudioCap.hpp"
	

void my_callback(void* buffer, uint32_t samples)
{
	write(1, buffer, samples*2); //2 bytes per sample
}

int main(int argc, char **argv)
{
	int t;
	if(argc == 2)
		t = (int)(strtod(argv[1], NULL));
	if(t <= 0 || t > 50)
		t = 10;
	t *= 1000000;
	AudioCap ac;
	ac.set_callback(my_callback);
	ac.start();
	usleep(t);
	ac.shutdown();
return 0;
}

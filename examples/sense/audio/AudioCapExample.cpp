#include "sense/audio/AudioCap.hpp"

void my_callback(void* buffer, uint32_t size)
{
	write(1, buffer, size);
}


int main(int argc, char **argv)
{
	AudioCap ac("default", 16000, false, AudioCap::fmt_bit_width::WORD, false, my_callback);
	ac.start();
	usleep(5000000);
	ac.stop();
	usleep(5000000);
	ac.start();
	usleep(5000000);
return 0;
}

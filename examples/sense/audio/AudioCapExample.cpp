#include "sense/audio/AudioCap.hpp"

void my_callback(void* buffer, uint32_t size)
{
	write(1, buffer, size);
}


int main(int argc, char **argv)
{
	AudioCap ac("default", 16000, SND_PCM_FORMAT_S16_LE, my_callback);
	while (true) ;
return 0;
}

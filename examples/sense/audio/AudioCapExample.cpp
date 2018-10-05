#include "sense/audio/AudioCap.hpp"

void my_callback(void* buffer, uint32_t size)
{
	write(1, buffer, size);
}


int main(int argc, char **argv)
{
	AudioCap ac;
	ac.set_callback(my_callback);
	ac.start();
	usleep(5000000);
	ac.stop();
	usleep(5000000);
	ac.start();
	usleep(5000000);
return 0;
}

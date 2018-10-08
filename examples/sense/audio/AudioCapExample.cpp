#include "sense/audio/AudioCap.hpp"


void my_callback(void* buffer, uint32_t size)
{
	write(1, buffer, size);
}


int main(int argc, char **argv)
{
	int t;
	if(argc == 2)
		t = (int)(strtod(argv[1], NULL));
	if(t == 0 || t > 50)
		t = 10;
	t *= 1000000;
	AudioCap ac;
	ac.set_callback(my_callback);
	ac.start();
	usleep(t);
	ac.shutdown();
return 0;
}

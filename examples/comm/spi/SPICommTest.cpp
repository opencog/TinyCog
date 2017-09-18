/*
   Project: OpenCogER
   File: SPICommTest.cpp
   Author: Dagim Sisay <dagiopia@gmail.com>
   Date: September 2017
*/

#include "SpiComm.hpp"

int main(int argc, char **argv)
{
	SpiComm spi_comm("/dev/spidev0.0", SpiComm::spi_mode::MODE0, 50000);
	if ( !spi_comm.okay )
	{
		printf("Error Connecting To SPI Device\n");
		return 1;
	}
	std::string data, ret;
	data = "Hello, SPI";
	ret = spi_comm.send_data(data);
	printf("Data Sent Size=%d\nRet=%s\n", ret.size(), ret.c_str());

return 0;
}

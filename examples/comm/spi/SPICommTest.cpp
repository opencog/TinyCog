/*
   Project: OpenCogER
   File: SPICommTest.cpp
   Author: Dagim Sisay <dagiopia@gmail.com>
   Date: September 2017
*/

#include "SpiComm.hpp"

/*
   This program sends the "Hello, SPI" text through the SPI interface.
   The data is transferred through the MOSI pin and it reads on the MISO 
   line after doing so. 
   So one can short the two lines so that a loop will allow the sent text
   to be received. 

   This hasn't been with another SPI slave device so it's not guranteed to work. 
*/

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
	for (int i = 8 ; i < ret.size(); i++)
		printf("%c", ret[i]);
	printf("\n");


return 0;
}

/*
  Project: OpenCogER
  File: SpiComm.hpp
  Author: Dagim Sisay <dagiopia@gmail.com>
  Date: September 2017
*/

#ifndef _SPI_MASTER_HPP_
#define _SPI_MASTER_HPP_


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <string.h>
#include <map>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define PACKET_SIZE_L_IDX 2
#define PACKET_SIZE_H_IDX 3
#define DEBUG 1

class SpiComm
{
    public:
        /*
	  Mode 0: CPHA=0 CPOL=0  ;  idle state 0, sample on low-high
	  Mode 1: CPHA=1 CPOL=0  ;  idle state 0, sample on high-low
	  Mode 2: CPHA=0 CPOL=1  ;  idle state 1, sample on high-low
	  Mode 3: CPHA=1 CPOL=1  ;  idle state 1, sample on low-high
	*/
    	enum spi_mode{MODE0, MODE1, MODE2, MODE3};
    	SpiComm(std::string device, spi_mode sm, uint16_t freq);
	~SpiComm();
	bool okay;
	std::string send_data(std::string data);

	/* #TODO Inlcude some GPIO code here that will behave like an interrupt 
	   for reading on the spi comm. The dr_roboto will issue a 
	   SLAVE_DATA_READY signal on one of the gpio pins that should 
	   trigger a read on the SPI. 
	*/
    
    private:
	int fd;       
	std::string dev;    //spi device
	uint8_t mode; //spi mode
	uint8_t bpw;  //bits per word
	uint16_t speed;//sclk speed 

	void exit_p(std::string msg);
	void load_map();
	void setup();
	void sigint_handler(int SIG);

};


#endif // _SPI_MASTER_HPP_

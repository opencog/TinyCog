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
#include <wiringPi.h>
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
    	SpiComm(std::string, spi_mode, uint16_t);
	//r_sig should be rpi gpio pin number in BCM numbering
	//this mode requries root when running
	SpiComm(std::string, spi_mode, uint16_t, uint8_t);
	~SpiComm();
	bool okay;
	std::string send_data(std::string, bool);

    
    private:
	int fd;       
	std::string dev;    //spi device
	uint8_t mode; //spi mode
	uint8_t bpw;  //bits per word
	uint16_t speed;//sclk speed 

	void exit_p(std::string);
	void load_map();
	void setup(std::string, spi_mode, uint16_t);
	std::string spi_send(uint8_t*, size_t);
	void int_handler(void);
	void sigint_handler(int);

};


#endif // _SPI_MASTER_HPP_

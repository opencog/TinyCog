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
#define DBG 1

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
	enum body_part{EYEBROW, EYELID, MOUTH, CHEEK, VHEAD, HHEAD, ARM};
    	SpiComm(std::string device, spi_mode sm, uint16_t freq);
	//r_sig should be rpi gpio pin number in BCM numbering
	//this mode requries root when running
	SpiComm(std::string device, spi_mode sm, uint16_t freq, uint8_t r_sig);
	~SpiComm();
	bool okay;
	std::string gesture_control(body_part bp, uint8_t value, uint8_t time);
	std::string send_data(std::string data);

    
    private:
	int fd;       
	std::string dev;    //spi device
	uint8_t mode; //spi mode
	uint8_t bpw;  //bits per word
	uint16_t speed;//sclk speed 
	void exit_p(std::string);
	void load_map();
	void setup(std::string, spi_mode, uint16_t);
	std::string spi_send(uint8_t *packet, size_t packet_len);
	void int_handler(void);
	void sigint_handler(int);

};


#endif // _SPI_MASTER_HPP_

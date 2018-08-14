/*
  Project: OpenCogER
  File: SpiComm.cpp
  Author: Dagim Sisay
  Licence: AGPL
  Date: September 2017
*/

#include "SpiComm.hpp"


SpiComm::SpiComm(std::string device, spi_mode sm, uint16_t freq)
{
	setup(device, sm, freq);
}

SpiComm::SpiComm(std::string device, spi_mode sm, uint16_t freq, uint8_t r_sig)
{
	if(wiringPiSetupGpio() < 0)
		exit_p("Couldn't Initiate WiringPi");
	//if(wiringPiISR(r_sig, INT_EDGE_RISING, &SpiComm::int_handler))
		exit_p("Couldn't Setup GPIO Interrupt");
	setup(device, sm, freq);
}

void SpiComm::setup(std::string device, spi_mode sm, uint16_t freq)
{
	okay = false;
	mode = 0;
	dev = device;
	speed = freq;
	bpw = 8;
	switch (sm) {
		case MODE0: break;
		case MODE1:
			mode |= SPI_CPHA;
			break;
		case MODE2:
			mode |= SPI_CPOL;
			break;
		case MODE3:
			mode |= SPI_CPHA;
			mode |= SPI_CPOL;
			break;
	}
	
	int ret;
	fd = open(device.c_str(), O_RDWR);
	if(fd < 0)
		exit_p("Couldn't Open Device");
	if( ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1)
		exit_p("Couldn't Set Spi Mode");
	if( ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bpw) == -1)
		exit_p("Couldn't Set Bits Per Word");
	if( ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &freq) == -1)
		exit_p("Couldn't Set SPI Clock Speed");
	
	okay = true;
}


SpiComm::~SpiComm()
{
	close(fd);
}

/*
	gesture control
	params:
		body_part: from enum body_part
		value: value between 0-100 for motor position
		time: time to take when moving from current pos to requesed pos
			in seconds (1 - 10)
*/

std::string SpiComm::gesture_control(body_part bp, uint8_t value, uint8_t time)
{
	size_t packet_len = 4;
	uint8_t packet[packet_len];
	packet[0] = 0x80;
	packet[1] = bp;
	if(0 <= value <= 100)
		packet[2] = value;
	else
		packet[2] = 0;
	if(0 <= time <= 10)
		packet[3] = time;
	else 
		packet[3] = 5;
	return spi_send(packet, packet_len);
}

/*
	
*/
std::string SpiComm::send_data(std::string data)
{
	size_t len = data.size();
	uint8_t *packet;
	uint8_t header_len = 8;
	uint8_t header[header_len] = {0xAA, 0x55, 0x0, 0x0, 0x01, 0x0, 0x0, 0x0};
	size_t packet_len = len+header_len;
	header[PACKET_SIZE_L_IDX] = len & 0xff;
	header[PACKET_SIZE_H_IDX] = len >> 8;
	packet = new uint8_t[packet_len];
	
	/* Attach Header */
	for (int i = 0; i < 8; i++)
		packet[i] = header[i];
	/* Attach Data */
	for(int i = 8; i < packet_len; i++)
		packet[i] = (uint8_t)data.at(i-8);
	
	if(DEBUG){
		printf("Data in Packet:\n");
		for(int i = 0; i < packet_len; i++)
			printf("%.2X ", packet[i]);
		printf("\n");
		printf("Size of Packet: %d\nPacket Content: %s\n", 
			packet_len, packet);
	}
	return spi_send(packet, packet_len);
}


std::string SpiComm::spi_send(uint8_t *packet, size_t packet_len)
{
	uint8_t *ret_buffer;
	ret_buffer = new uint8_t[packet_len];
	
	spi_ioc_transfer tr;
	tr.tx_buf = (unsigned long) &packet;
	tr.rx_buf = (unsigned long) &ret_buffer;
	tr.len = packet_len;
	tr.delay_usecs = 0;
	tr.speed_hz = speed;
	tr.bits_per_word = bpw;

	int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if(DEBUG) printf("Ret=%d\n", ret);
	if(ret < 1)
		exit_p("Couldn't Send The Message");
	
	// XXX There is a bug here. Idk what it is but if no data is received
	// then there is segfault
	std::string ret_data;
	for (int i = 0; i < packet_len; i++){
		//ret_data.append<char>(1, ret_buffer[i]);
                ret_data += ret_buffer[i];
		//printf("%.2X ", ret_buffer[i]);
	}
	
	if(DEBUG)
		printf("\nFinal Data Size = %d\n%s\n", ret_data.size(), 
			ret_data.c_str());
	return ret_data;
}


void SpiComm::int_handler(void)
{
	//TODO this should trigger a read on the spi interface.
	printf("Interrupt Received!!");
}



void SpiComm::sigint_handler(int SIG)
{
	printf("\nSIGINT Caught\nExiting...\n");
	close(fd);
	exit(1);
}

void SpiComm::exit_p(std::string msg)
{
	printf("ERROR!\n%s\n", msg.c_str());
	if(fd > 0)
		close(fd);
	exit(1);
}

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


std::string SpiComm::send_data(std::string data)
{
	size_t len = data.size();
	uint8_t *packet;
	size_t packet_len = len + 8;
	uint8_t header[8] = {0xAA, 0x55, 0x0, 0x0, 0x01, 0x0, 0x0, 0x0};
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
		printf("Size of Packet: %d\nPacket Content: %s\n", packet_len+8, packet);
	}

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
	printf("Ret=%d\n", ret);
	if(ret < 1)
		exit_p("Couldn't Send The Message");
	
	printf("The Primitive Way:\n");
	for(ret = 0; ret < packet_len; ret++){
		if(!(ret % 6))
			printf("\n");
		printf("%.2X ", ret_buffer[ret]);
	}
	printf("\n");
	
	printf("\nMy Way:\n");
	std::string ret_data;
	for (int i = 0; i < packet_len; i++){
		ret_data.append<int>(1, ret_buffer[i]);
		printf("%.2X ", ret_buffer[i]);
	}
	printf("\nFinal Data Size = %d\n%s\n", ret_data.size(), ret_data.c_str());
	return ret_data;
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

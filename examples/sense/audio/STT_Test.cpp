/*
 * File: STT_Test.cpp
 * Author: Dagim Sisay <dagiopia@gmail.com>
 * License: AGPL
 * Date: May, 2018
*/

#include <stdio.h>
#include <signal.h>

#include "sense/audio/STT.hpp"


void print_help()
{
	printf("provide args\n");
	printf("\t1, AudioCap - Socket(client)\n");
	printf("\t2, AudioCap - Socket(server)\n");
	printf("\t3, AudioCap - Callback\n");
	printf("\t4, Ad - Socket(client)\n");
	printf("\t5, Ad - Socket(server)\n");
	printf("\t6, Ad - Callback\n");
	exit(0);
}


void sigint_handler(int sig)
{
    printf("Caught SIGINT! Exiting...\n");
    tcstt_close();
    exit(0);
}

void stt_callback(const char *text)
{
    printf("Human Said: %s\n", text);
}


int main(int argc, char **argv)
{
	if(argc == 1) {
		print_help();
	}
	signal(SIGINT, sigint_handler);
	switch((int)strtod(argv[1], NULL)) {
		case 1:
	printf("1\n");
			tcstt_init(false, "/tmp/tc_stt", true);
			usleep(10000000);
			break;
		case 2:
	printf("2\n");
			tcstt_init(false, "/tmp/tc_stt", false);
			usleep(10000000);
			break;
		case 3:
	printf("3\n");
			tcstt_init(false, stt_callback);
			usleep(10000000);
			break;
		case 4:
	printf("4\n");
			tcstt_init(true, "/tmp/tc_stt", true);
			break;
		case 5:
	printf("5\n");
			tcstt_init(true, "/tmp/tc_stt", false);
			break;
		case 6:
	printf("6\n");
			tcstt_init(true, stt_callback);
			break;
		default:
			print_help();
	}
return 0;
}

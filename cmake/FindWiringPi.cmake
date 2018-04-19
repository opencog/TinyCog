# Project: OpenCogER
# File: FindWiringPi.cmake
# Author: Dagim Sisay <dagiopia@gmail.com>
# Licence: AGPL
# Date: September 2017

FIND_LIBRARY(WIRINGPI_LIB NAMES wiringPi PATH /usr/lib /usr/local/lib)
FIND_PATH(WIRINGPI_INCLUDE_DIR wiringPi.h PATHS /usr/include /usr/local/include)
IF(WIRINGPI_LIB AND WIRINGPI_INCLUDE_DIR)
	SET(WIRINGPI_FOUND TRUE)
ENDIF(WIRINGPI_LIB AND WIRINGPI_INCLUDE_DIR)

IF(WIRINGPI_FOUND)
	MESSAGE(STATUS "Found WiringPi: ${WIRINGPI_LIB}")
ELSE(WIRINGPI_FOUND)
	MESSAGE(STATUS "WiringPi Not Found")
ENDIF(WIRINGPI_FOUND)

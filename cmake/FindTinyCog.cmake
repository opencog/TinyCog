#		Find TinyCog on the system.
#        This code sets the following variables:
#
#	        - TINYCOG_INCLUDE_DIR 	
#	        - TINYCOG_LIBRARIES		   
#
#======================================================================

FIND_LIBRARY(SENSOR_VISION NAMES sensor_vision  HINTS /usr/lib/TinyCog /usr/local/lib/TinyCog)
FIND_LIBRARY(SENSOR_AUDIO NAMES sensor_audio  HINTS /usr/lib/TinyCog /usr/local/lib/TinyCog)
FIND_LIBRARY(ACT_AUDIO NAMES act_audio HINTS /usr/lib/TinyCog /usr/local/lib/TinyCog)
FIND_LIBRARY(NETWORK NAMES networkr HINTS /usr/lib/TinyCog /usr/local/lib/TinyCog)
FIND_LIBRARY(SPI_COMM NAMES spicomm HINTS /usr/lib/TinyCog /usr/local/lib/TinyCog)
FIND_LIBRARY(DR_ROBOTO NAMES dr_roboto HINTS /usr/lib/TinyCog /usr/local/lib/TinyCog)

set(TINYCOG_LIBRARIES ${SENSOR_VISION} ${SENSOR_AUDIO} ${ACT_AUDIO} ${NETWORK} ${SPI_COMM} ${DR_ROBOTO})

find_path(VISION_INCLUDE_DIR1 BoxTracker.hpp
		  HINTS /usr/include/TinyCog/sense/vision /usr/local/include/TinyCog/sense/vision)

find_path(VISION_INCLUDE_DIR2 RaspiVision.hpp
		  HINTS /usr/include/TinyCog/sense/vision /usr/local/include/TinyCog/sense/vision)

find_path(VISION_INCLUDE_DIR3 Vision.hpp
		  HINTS /usr/include/TinyCog/sense/vision /usr/local/include/TinyCog/sense/vision)


find_path(TTS_INCLUDE_DIR FestivalTTS.hpp
		  HINTS /usr/include/TinyCog/act/audio /usr/local/include/TinyCog/act/audio)

find_path(NETWORKR_INCLUDE_DIR StringServerUDP.hpp
		  HINTS /usr/include/TinyCog/network /usr/local/include/TinyCog/network)


find_path(COMM_INCLUDE_DIR SpiComm.hpp
		  HINTS /usr/include/TinyCog/comm/spi /usr/local/include/TinyCog/comm/spi)


set(TINYCOG_INCLUDE_DIR ${VISION_INCLUDE_DIR1} ${VISION_INCLUDE_DIR2} ${VISION_INCLUDE_DIR3} ${TTS_INCLUDE_DIR} ${NETWORKR_INCLUDE_DIR} ${COMM_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TinyCog DEFAULT_MSG SENSOR_VISION SENSOR_AUDIO ACT_AUDIO NETWORK SPI_COMM DR_ROBOTO VISION_INCLUDE_DIR1 VISION_INCLUDE_DIR2 VISION_INCLUDE_DIR3 TTS_INCLUDE_DIR NETWORKR_INCLUDE_DIR COMM_INCLUDE_DIR)

mark_as_advanced(TINYCOG_INCLUDE_DIR TINYCOG_LIBRARIES)






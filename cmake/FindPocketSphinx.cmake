# Project: TinyCog
# File: FindPocketSphinx.cmake
# Author: Dagim Sisay <dagiopia@gmail.com>
# Licence: AGPL
# Date: May, 2018

# This config file sets the following variables
#    POCKETSPHINX_LIBRARIES              PocketShinx libs to link to
#    POCKETSPHINX_INCLUDE_DIRS           Include dirs for pocketsphinx and sphinx base
#    POCKETSPHINX_MODELDIR               Sphinx dict, lang and acoustic models folder

FIND_LIBRARY(POCKETSPHINX_LIBRARIES NAMES pocketsphinx sphinxbase 
                                    PATH /usr/lib /usr/local/lib )
FIND_PATH(POCKETSPHINX_INCLUDE_DIRS pocketsphinx.h sphinx_config.h 
                            PATHS /usr/include /usr/local/include)
IF(POCKETSPHINX_LIBRARIES AND POCKETSPHINX_INCLUDE_DIRS)
	SET(POCKETSPHINX_FOUND TRUE)
ENDIF(POCKETSPHINX_LIBRARIES AND POCKETSPHINX_INCLUDE_DIRS)

IF(POCKETSPHINX_FOUND)
	MESSAGE(STATUS "Found PocketSphinx: ${POCKETSPHINX_LIBRARIES}")
ELSE(POCKETSPHINX_FOUND)
	MESSAGE(STATUS "PocketSphinx Was Not Found")
ENDIF(POCKETSPHINX_FOUND)

# Project: TinyCog
# File: FindPocketSphinx.cmake
# Author: Dagim Sisay <dagiopia@gmail.com>
# Licence: AGPL
# Date: May, 2018

# This config file sets the following variables
#    POCKETSPHINX_LIBRARY                PocketShinx libs
#    SPHINX_BASE_LIBRARY                 SphinxBase libs
#    SPHINXAD_LIBRARY                    Sphinx audio interface
#    POCKETSPHINX_LIBRARIES              All the above three libs
#    POCKETSPHINX_INCLUDE_DIRS           Include dirs for pocketsphinx and sphinx base
#    POCKETSPHINX_MODEL_DIR              Sphinx dict, lang and acoustic models dir

FIND_PACKAGE(PkgConfig REQUIRED)

FIND_LIBRARY(POCKETSPHINX_LIBRARY NAMES pocketsphinx
                                    PATH /usr/lib /usr/local/lib )
FIND_LIBRARY(SPHINXBASE_LIBRARY NAMES sphinxbase 
                                    PATH /usr/lib /usr/local/lib )
FIND_LIBRARY(SPHINXAD_LIBRARY NAMES sphinxad
                                    PATH /usr/lib /usr/local/lib )

IF( POCKETSPHINX_LIBRARY AND SPHINXBASE_LIBRARY AND SPHINXAD_LIBRARY)
    SET(POCKETSPHINX_LIBRARIES 
              ${POCKETSPHINX_LIBRARY}
	      ${SPHINXBASE_LIBRARY}
	      ${SPHINXAD_LIBRARY} )
ENDIF (POCKETSPHINX_LIBRARY AND SPHINXBASE_LIBRARY AND SPHINXAD_LIBRARY)

FIND_PATH(POCKETSPHINX_INCLUDE_DIR pocketsphinx.h
                              PATHS /usr/include 
			            /usr/local/include 
                              PATH_SUFFIXES pocketsphinx)


FIND_PATH(SPHINXBASE_INCLUDE_DIR sphinx_config.h
                              PATHS /usr/include 
			            /usr/local/include 
                              PATH_SUFFIXES sphinxbase)

IF(POCKETSPHINX_INCLUDE_DIR AND SPHINXBASE_INCLUDE_DIR)
    SET(POCKETSPHINX_INCLUDE_DIRS 
               ${POCKETSPHINX_INCLUDE_DIR} 
	       ${SPHINXBASE_INCLUDE_DIR} )
ENDIF(POCKETSPHINX_INCLUDE_DIR AND SPHINXBASE_INCLUDE_DIR)

IF(POCKETSPHINX_LIBRARIES AND POCKETSPHINX_INCLUDE_DIRS)
	SET(POCKETSPHINX_FOUND TRUE)
	EXECUTE_PROCESS( COMMAND ${PKG_CONFIG_EXECUTABLE} 
	                               pocketsphinx --variable=modeldir
                                 OUTPUT_VARIABLE POCKETSPHINX_MODEL_DIR
		                 OUTPUT_STRIP_TRAILING_WHITESPACE )
ENDIF(POCKETSPHINX_LIBRARIES AND POCKETSPHINX_INCLUDE_DIRS)


IF(POCKETSPHINX_FOUND)
	MESSAGE(STATUS "Found PocketSphinx: ${POCKETSPHINX_LIBRARIES}")
ELSE(POCKETSPHINX_FOUND)
	MESSAGE(STATUS "PocketSphinx Was Not Found")
ENDIF(POCKETSPHINX_FOUND)

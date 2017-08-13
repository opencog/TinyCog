#		Find EST on the development system.
# This module finds if Festival is installed and determines where the
# include files and libraries are. It also determines what the name of
# the library is. This code sets the following variables:
#
#	EST_INCLUDE_DIR 	- path where EST.h is found
#	EST_LIBRARIES		- path to the EST library
#
#======================================================================
# Copyright (c) 2017 Mandeep Singh Bhatia
#
# This module is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

find_path(EST_INCLUDE_DIR EST.h
		  HINTS /usr/include/speech_tools /usr/local/include/speech_tools
		  PATH_SUFFIXES speech_tools)

find_library(EST_LIBRARIES estbase estools eststring
		   HINTS /usr/lib/ usr/local/lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(EST DEFAULT_MSG
    EST_LIBRARIES EST_INCLUDE_DIR)


mark_as_advanced(EST_INCLUDE_DIR EST_LIBRARIES)

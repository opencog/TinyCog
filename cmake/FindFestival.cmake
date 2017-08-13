#		Find Festival on the development system.
# This module finds if Festival is installed and determines where the
# include files and libraries are. It also determines what the name of
# the library is. This code sets the following variables:
#
#	FESTIVAL_INCLUDE_DIR 	- path where festival.h is found
#	FESTIVAL_LIBRARIES		- path to the Festival library
#
#======================================================================
# Copyright (c) 2014 Gabriel Araujo <gabriel.araujo.5000@gmail.com>
#
# This module is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

find_path(FESTIVAL_INCLUDE_DIR festival.h
		  HINTS /usr/include/festival /usr/local/include/festival
		  PATH_SUFFIXES festival)

find_library(FESTIVAL_LIBRARIES Festival
		   HINTS /usr/lib/ usr/local/lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Festival DEFAULT_MSG
    FESTIVAL_LIBRARIES FESTIVAL_INCLUDE_DIR)


mark_as_advanced(FESTIVAL_INCLUDE_DIR FESTIVAL_LIBRARIES)

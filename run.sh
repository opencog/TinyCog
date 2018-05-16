#!/bin/bash
export LTDL_LIBRARY_PATH=$(pwd)/build
cd build
guile -l ../behavior/dr_roboto.scm

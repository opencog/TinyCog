#!/bin/bash
cd build
export LTDL_LIBRARY_PATH=$(pwd)
guile -l ../behavior/dr_roboto.scm

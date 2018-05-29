#!/bin/bash
export LTDL_LIBRARY_PATH=$(pwd)/build
guile -l dr-roboto.scm

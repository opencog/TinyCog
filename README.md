# OpenCogER

This repo was started by @yantrabuddhi to be a collection of functionalities a toy robot needs on a small 
system such as a Raspberry Pi computer. The goal is to have some detectors that are as efficient as possible 
on a rpi3 computer feeding a behavior program that reacts based on data from these detectors.
The sense functions are suppose to sense stuff such as faces, emotions, gesture and salient points. 

Hardware-wise, a RPI3 computer, a Pi Camera V2 and a USB Microphone are required and are also the constraints
of the project.

The current code has a few detectors and there's a lot more to add. Most importantly, it needs to have an 
interface for [Ghost](https://github.com/opencog/opencog/tree/master/opencog/ghost) which would handle the behavior
of the robot. 

## Setup

Everything here is meant to run on a rpi3 computer but one can also compile everything on a desktop computer. 

### Install 

Need to have these whether on desktop or rpi
* [Opencv-3.x](https://github.com/opencv/opencv/releases/latest)
* [Opencv_contrib-3.x] (https://github.com/opencv/opencv_contrib/releases)
* [Dlib] (https://github.com/davisking/dlib/releases/latest)
* [Festival] (http://festvox.org/festival/)
* 
On rpi3 only
* [Raspicam] (https://sourceforge.net/projects/raspicam/files/?) > v1.5
* [WiringPi] (http://wiringpi.com/download-and-install/)

Use cmake for building. 
```
    cd to OpenCogEr dir
    mkdir build
    cd build
    cmake .. 
    make
```


## ToDo

* Offline Speech (command) Recognition (shouldn't be heavy on the hardware)
* Ghost interface
* Ghost rules
* Stories for a specific identity we need the robot to have
* Access to Singnet applications

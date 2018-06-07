# TinyCog

A collection of speech, vision, and movement functionalities 
aimed at small or toy robots on embedded systems, such as the 
Raspberry Pi computer. High level reasoning, language understanding, 
language gneration and movement planning is provided by OpenCog.

The current hardware platform requires an RPI3 computer, a Pi 
Camera V2 and a USB Microphone; other sensor/detector components 
are planned.

The current software functions include face detection, emotion 
recognition, gesture analysis, speech-to-text and text-to-speech 
subsystems. All high-level founction is provided by OpenCog, and 
specifically by the 
[Ghost](https://github.com/opencog/opencog/tree/master/opencog/ghost) 
scripting system -- ghost is able to process sensory input, 
and provide coordinated chatbot and movement abilities. 

## Setup

Everything here is meant to run on a rpi3 computer; one can also 
compile everything on a standard Linux desktop computer. 

### Install 

Need to have these whether on desktop or rpi
* [Opencv-3.x](https://github.com/opencv/opencv/releases/latest)
* [Opencv_contrib-3.x](https://github.com/opencv/opencv_contrib/releases)
* [Dlib](https://github.com/davisking/dlib/releases/latest)
* [Festival](http://festvox.org/festival/)
* [Guile 2.2](https://www.gnu.org/software/guile/download/)
* [PocketSphinx](https://cmusphinx.github.io/wiki/tutorialpocketsphinx/#installation-on-unix-system)
On rpi3 only
* [Raspicam](https://sourceforge.net/projects/raspicam/files/?) > v1.5
* [WiringPi](http://wiringpi.com/download-and-install/)

Use cmake for building. 
    ```
        cd to TinyCog dir
        mkdir build
        cd build
        cmake .. 
        make
    ```

### Running

* Start relex [server](https://github.com/opencog/relex#opencog-serversh)
* run [run.sh](run.sh)
    ```
        $ ./run.sh
    ```
* A guile shell starts up where you can test behavorial functions with simulated functions.
    ```
        guile> (test-set-face-event)
    ```
* In another terminal, connect to port 5555 via telnet to input speech
    ```
        $ telnet localhost 5555
    ```
* You can see response output either from the guile instance above or by telneting to port 6666
    ```
        $ telnet localhost 6666
    ```

## Implementation

### Sensors

Sensors are a camera and microphones. The camera for face detection 
-> face landmark -> facial expression  and emotion and also for hand 
detection -> convexity defects -> fingers count and gestures. The 
microphone for STT and sound source localization. Some of the sensor 
programs such as the face, hand and voice activity detection can run 
on the PI without much stress on the hardware but other functionalities 
like emotion and and speech recogntion should be implemented as services 
from a server possibly from singnet. Currently, STT is implemented using
pocketsphinx. It's not ideal but can be used for a very limited range of
commands and simple conversation. 

### Act

The robot should also act as well as sense. It must speak and move around. 
The speech synthesis utilizes festival. The code is in [act/audio](act/audio).
Movement was intended to be with SPI communication with the hardware but 
that has changed. However the spi interface is in [comm/spi](comm/spi)

### Behavior

* Code connecting the behavior module to the  STT is found in [socket/socket.scm](behavior/socket/socket.scm)
* Behavior rules are in [behavior/](behavior/)
    * Ghost rules that list all syntax found in [behavior/test.top](behavior/test.top)
    * For non-speech triggered actions, OpenPsi's dynamic module is used to indirectly trigger ghost rules. 
        ```
            event ==> phony speech                          (Dynamic OpenPsi rule)
	    phony speech ==> speech or some action          (Ghost rule)
        ```
    * Events and event monitoring functions are in [beavior/events.scm](behavior/events.scm)
    * Dynamic OpenPsi rules are found in [behavior/interaction-rules.scm](behavior/interaction-rules.scm)
    * Functions/actions used by Ghost rules are found at [behavior/functions.scm](behavior/functions.scm)
    * Testing functions in [behavior/test.scm](behavior/test.scm)
* Scheme-C++ binding in [dr-roboto/dr-roboto.cpp](dr-roboto/dr-roboto.scm)
* Scratch interface in [behavior/scratch](behavior/scratch/)

## ToDo
* Emotion recognition
* Improve STT
* Ghost rules
* Stories for a specific identity we need the robot to have
* Access to Singnet applications

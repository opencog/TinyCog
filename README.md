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

A fully prepped raspbian image is available 
[here](http://144.76.153.5/rpi3/64bit/)

Use xzcat to clone the image as shown here replacing sdX with your device.
```
    xzcat oc-debian-stretch-arm64.img.xz | sudo dd of=/dev/sdX
```

When you first boot with this image, and login with the default credentials,
it automatically expands the filesystem to occupy the entire /
partition and then it reboots. 
This is a 64bit Debian Stretch OS for RPI3 which means that PiCamera driver
is not available. A USB camera should be used for this image.

The default credentials:
```
    Username: oc
    Password: opencog
```

The image contains the opencog version at the time of it's building
and other libraries such as opencv and dlib (3.4 and 19.15).
To see the opencog commit version, pkg-config can be used.
```    
    pkg-config --variable=cogutil opencog    #shows cogutil commit version
    pkg-config --variable=atomspace opencog    #shows opencog commit version
    pkg-config --variable=opencog opencog    #shows opencog commit version
```

There is a problem with this image, no driver for piCamera as it's not available
as a 64bit binary. 

### Install 

Need to have these whether on desktop or rpi
* [Opencv-3.2](https://github.com/opencv/opencv/releases/latest)
* [Opencv_contrib-3.2](https://github.com/opencv/opencv_contrib/releases)
* [Dlib](https://github.com/davisking/dlib/releases/latest)
* [Festival](http://festvox.org/festival/)
* [Guile 2.2](https://www.gnu.org/software/guile/download/)
* [PocketSphinx](https://cmusphinx.github.io/wiki/tutorialpocketsphinx/#installation-on-unix-system)
On rpi3 only
* [Raspicam](https://sourceforge.net/projects/raspicam/files/?) > v1.5
* [WiringPi](http://wiringpi.com/download-and-install/)

Use cmake for building. 
Default build mode is Debug mode. Set CMAKE_BUILD_TYPE to Release to disable debug mode.
For the emotion recognition service you should set the variable 
SERVER_ADDRESS to "34.216.72.29:6205"
```
    cd to TinyCog dir
    mkdir build
    cd build
    cmake ..  # -DCMAKE_BUILD_TYPE=Release -DSERVER_ADDRESS="34.216.72.29:6205" 
    make
```


### Testing

* To test the sensors from the guile shell, run the following from within the build dir
which opens up the camera and does a live view of the camera with markings for the sensors. 
```
    $ ./TestDrRoboto.scm
```
* To test from a video file instead of a camera, run the folloiwng way
```
    $ ./TestDrRoboto.scm -- <video_file_path>
```

### Running

* Start relex [server](https://github.com/opencog/relex#opencog-serversh)
* run [run.sh](run.sh)
```
    $ guile -l dr-roboto.scm
```
* In another terminal, connect to port 5555 via telnet to input speech
```
    $ telnet localhost 5555
```


## Implementation

### Overall Description

The [dr-roboto.cpp](dr-roboto/dr-roboto.cpp.in) file is compiled to a guile
extension which is loaded with the scheme [dr-roboto.scm](dr-roboto.scm) file.
This guile extension is written in C++ and it's main job is to open the camera
and sense stuff. When the scheme program loads the extension, the first thing
it does is it sends the address of its atomspace to the extension so that the
two can share an atomspace. Then the sensors are started which is a loop run in
a separate thread that just collects information and places them in the atomspace.
Most sensory values are stored with Atomspace Values in the following format:
```
    Value
        ConceptNode "position"
        ConceptNode "face_x"
        FloatValue X Y
```

The scheme program [dr-roboto.scm](dr-roboto.scm) includes the
[behavior/behavior.scm](behavior/behavior.scm) code that contains a very small
model of OpenCog behavior tree. The behavior is a looking behavior which first
goes through checking if there is a face, if there is only one then just look at
that one, if there are more than one then check if one of them is smiling, if not,
check if any of them has a non-neutral facial expression, if not just look at
one of the faces randomly. If there are no faces in view then just look at the
salient point. The behavior tree calls functions that simply check the atomspace
for the information they require.
The [behavior/behavior.scm](behavior/behavior.scm) file also loads the ghost
scripts located [here](ghost/)
When the behavior program wants to command the Professor Einstein robot of Hanson
Robotics, it calls functions defined in [cmd_einstein.scm](act/cmd_einstein.scm)
This program connects to the Professor Einstein robot through its socket api
and sends it commands.
The [fuctions.scm](behavior/functions.scm) file contains some utility functions
used by other scheme source files such as converting ghost results which are a
list of WordNodes to a single string to be spoken by the robot and mapping of
values between the image dimensions and the robot's pan/tilt limit.



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

For Hanson Robotic's Professor Einstein robot, the 
[cmd_einstein.scm](act/cmd_einstein.scm) file contains the code necessary
to command it.
The robot should also act as well as sense. It must speak and move around. 
The speech synthesis utilizes festival. The code is in [act/audio](act/audio).
Movement was intended to be with SPI communication with the hardware but 
that has changed. However the spi interface is in [comm/spi](comm/spi)

### Behavior

* Code connecting the behavior module to the  STT is found in [socket/socket.scm](socket/socket.scm)
* Behavior rules are in [behavior/](behavior/)
    * Ghost rules that list all syntax found in [ghost/](ghost/)
    * For non-speech triggered actions, an OpenCog behavior tree is used. 
    * Events and event monitoring functions are in [beavior/events.scm](behavior/behavior.scm)
    * Functions/actions used by various scheme code are found at [behavior/functions.scm](behavior/functions.scm)
* Scheme-C++ binding in [dr-roboto/dr-roboto.cpp](dr-roboto/dr-roboto.cpp.in)
* Scratch interface in [behavior/scratch](scratch/)

## ToDo
* Improve STT
* Ghost rules
* Stories for a specific identity we need the robot to have

cmake_minimum_required(VERSION 3.1)
project( OpenCogER )
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/" "/usr/local/lib/cmake/")
if(CMAKE_COMPILER_IS_GNUCXX)
    set(EXTRA_LIBS ${EXTRA_LIBS} pthread)
endif()
############Vision
find_package( OpenCV REQUIRED )
find_package( dlib REQUIRED )
find_package(raspicam QUIET)
include_directories("include" "include/sensor/vision")

if (raspicam_FOUND)
    set(RASPI_SUP sense/vision/capture/RaspiCamCapture.cpp)
endif()

add_library( sensor_vision  sense/vision/capture/CamCapture.cpp           
        ${RASPI_SUP}
        sense/vision/capture/FileCapture.cpp
        sense/vision/capture/ITColor2Gray.cpp
        sense/vision/capture/ITEqualizeHist.cpp
        sense/vision/detect/ITDetectFace.cpp
        sense/vision/detect/ITDetectSmile.cpp
	sense/vision/detect/FacialLandMark.cpp
        sense/vision/track/BoxTracker.cpp
        sense/vision/track/BoxTrackerThread.cpp)

target_link_libraries( sensor_vision ${OpenCV_LIBS} ${EXTRA_LIBS} dlib::dlib )

add_executable(VisionTest examples/sense/vision/VisionTest.cpp)
target_link_libraries(VisionTest sensor_vision ${OpenCV_LIBS} ${EXTRA_LIBS})

add_executable(VisionTestFace examples/sense/vision/VisionTestFace.cpp)
target_link_libraries(VisionTestFace sensor_vision ${OpenCV_LIBS} ${EXTRA_LIBS})

add_executable(FaceLandMarkTest examples/sense/vision/FaceLandMarkTest.cpp)
target_link_libraries(FaceLandMarkTest sensor_vision ${OpenCV_LIBS} ${EXTRA_LIBS} dlib::dlib)

if(raspicam_FOUND)
add_executable(RaspiVisionTest examples/sense/vision/RaspiVisionTest.cpp)
target_link_libraries(RaspiVisionTest sensor_vision ${raspicam_CV_LIBS} ${EXTRA_LIBS})

add_executable(RaspiVisionTestFace examples/sense/vision/RaspiVisionTestFace.cpp)
target_link_libraries(RaspiVisionTestFace sensor_vision ${raspicam_CV_LIBS} ${EXTRA_LIBS})   
endif(raspicam_FOUND)
configure_file(data/Haar_Cascades/haarcascade_frontalface_alt.xml haarcascade_frontalface_alt.xml COPYONLY)
configure_file(data/Haar_Cascades/haarcascade_smile.xml haarcascade_smile.xml COPYONLY)
configure_file(data/Facial_Landmark_Model/shape_predictor_68_face_landmarks.dat 
							shape_predictor_68_face_landmarks.dat COPYONLY)

install (TARGETS sensor_vision DESTINATION lib/OpenCogER)
install (FILES BoxTracker.hpp BoxTrackerThread.hpp
    CamCapture.hpp FileCapture.hpp ImageSource.hpp
    ImageTransform.hpp ITColor2Gray.hpp ITEqualizeHist.hpp
    ITDetectFace.hpp ITDetectSmile.hpp
    DESTINATION "include/OpenCogER/sense/vision")

########TTS
find_package(Festival REQUIRED)
find_package(EST REQUIRED)
include_directories("include" "include/act/audio" ${FESTIVAL_INCLUDE_DIR} ${EST_INCLUDE_DIR})
add_library( act_audio act/audio/FestivalTTS.cpp )
target_link_libraries( act_audio ${FESTIVAL_LIBRARIES} ${EST_LIBRARIES})

add_executable( exampleTTS examples/act/audio/exampleTTS.cpp)
target_link_libraries( exampleTTS act_audio )

install (TARGETS act_audio DESTINATION lib/OpenCogER)
install (FILES TTS.hpp FestivalTTS.hpp 
        DESTINATION "include/OpenCogER/act/audio" )
        
#########network
include_directories("include" "include/network")
add_library( networkr network/StringServerUDP.cpp)
target_link_libraries( networkr ${EXTRA_LIBS} )

add_executable( exUDPserver examples/network/exUDPserver.cpp)
target_link_libraries(exUDPserver networkr)

install (TARGETS networkr DESTINATION lib/OpenCogER)
install (FILES StringServerUDP.hpp DESTINATION "include/OpenCogER/network")

#######COMM
find_package(WiringPi REQUIRED)
include_directories("include/comm/spi" ${WIRINGPI_INCLUDE_DIR})
add_library(spicomm comm/spi/SpiComm.cpp)
target_link_libraries(spicomm ${WIRINGPI_LIB})

add_executable(SPICommTest examples/comm/spi/SPICommTest.cpp)
target_link_libraries(SPICommTest spicomm)

cmake_minimum_required(VERSION 3.1)
project( TinyCog )
set (CMAKE_INSTALL_PREFIX "/usr/local")
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/" "/usr/local/lib/cmake/")
if(CMAKE_COMPILER_IS_GNUCXX)
    set(EXTRA_LIBS ${EXTRA_LIBS} pthread)
endif()
############Vision
find_package( OpenCV REQUIRED )
find_package( dlib REQUIRED )
find_package( Protobuf REQUIRED )
find_package( raspicam QUIET )
find_package( Guile REQUIRED )
find_package( PkgConfig REQUIRED )

include_directories("include" ${GUILE_INCLUDE_DIRS})

if (raspicam_FOUND)
    set(RASPI_SUP sense/vision/capture/RaspiCamCapture.cpp)
endif()

add_library( sensor_vision sense/vision/capture/CamCapture.cpp           
	${RASPI_SUP}
	sense/vision/capture/FileCapture.cpp
	sense/vision/capture/ITColor2Gray.cpp
	sense/vision/capture/ITEqualizeHist.cpp
	sense/vision/detect/ITDetectFace.cpp
	sense/vision/detect/ITDetectSmile.cpp
	sense/vision/detect/ITDetectHand.cpp
	sense/vision/detect/FacialLandMark.cpp
	sense/vision/detect/FacialExpressions.cpp
	sense/vision/detect/FingersCount.cpp
	sense/vision/detect/DSaliency.cpp
	sense/vision/track/BoxTracker.cpp
   sense/vision/track/BoxTrackerThread.cpp
)

set_property(TARGET sensor_vision PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries( sensor_vision ${OpenCV_LIBS} ${EXTRA_LIBS} dlib::dlib )

add_library(tinycog_util util/util.cpp)


ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/comm/rpc)


add_executable(VisionTest examples/sense/vision/VisionTest.cpp)
target_link_libraries(VisionTest sensor_vision ${OpenCV_LIBS} ${EXTRA_LIBS})

add_executable(VisionTestFace examples/sense/vision/VisionTestFace.cpp)
target_link_libraries(VisionTestFace sensor_vision ${OpenCV_LIBS} ${EXTRA_LIBS})

add_executable(FaceLandMarkTest examples/sense/vision/FaceLandMarkTest.cpp)
target_link_libraries(FaceLandMarkTest sensor_vision ${OpenCV_LIBS} ${EXTRA_LIBS} dlib::dlib)

add_executable(NoddingTest examples/sense/vision/FaceNoddingDetection.cpp)
target_link_libraries(NoddingTest sensor_vision ${OpenCV_LIBS} ${EXTRA_LIBS} dlib::dlib tinycog_util)

add_executable(FingersCountTest examples/sense/vision/FingersCountTest.cpp)
target_link_libraries(FingersCountTest sensor_vision ${OpenCV_LIBS} ${EXTRA_LIBS})   

add_executable(SaliencyTest examples/sense/vision/SaliencyTest.cpp)
target_link_libraries(SaliencyTest sensor_vision ${OpenCV_LIBS} ${EXTRA_LIBS})   

add_executable(AllTest examples/sense/vision/AllTest.cpp)
target_link_libraries(AllTest sensor_vision ${OpenCV_LIBS} ${EXTRA_LIBS})  

if(raspicam_FOUND)

add_executable(RaspiVisionTest examples/sense/vision/RaspiVisionTest.cpp)
target_link_libraries(RaspiVisionTest sensor_vision ${raspicam_CV_LIBS} ${EXTRA_LIBS})

add_executable(RaspiVisionTestFace examples/sense/vision/RaspiVisionTestFace.cpp)
target_link_libraries(RaspiVisionTestFace sensor_vision ${raspicam_CV_LIBS} ${EXTRA_LIBS})   

add_executable(RaspiFaceLandMarkTest examples/sense/vision/RaspiFaceLandMarkTest.cpp)
target_link_libraries(RaspiFaceLandMarkTest sensor_vision ${raspicam_CV_LIBS} ${EXTRA_LIBS} dlib::dlib)

add_executable(RaspiFingersCountTest examples/sense/vision/RaspiFingersCountTest.cpp)
target_link_libraries(RaspiFingersCountTest sensor_vision ${raspicam_CV_LIBS} ${EXTRA_LIBS} dlib::dlib)

add_executable(RaspiSaliencyTest examples/sense/vision/RaspiSaliencyTest.cpp)
target_link_libraries(RaspiSaliencyTest sensor_vision ${raspicam_CV_LIBS} ${EXTRA_LIBS} dlib::dlib)


endif(raspicam_FOUND)

configure_file(data/Haar_Cascades/haarcascade_frontalface_alt.xml haarcascade_frontalface_alt.xml COPYONLY)
configure_file(data/Haar_Cascades/haarcascade_smile.xml haarcascade_smile.xml COPYONLY)
configure_file(data/Haar_Cascades/haarcascade_hand.xml haarcascade_hand.xml COPYONLY)
configure_file(data/Facial_Landmark_Model/shape_predictor_26_face_landmarks.dat 
							shape_predictor_26_face_landmarks.dat COPYONLY)

install (TARGETS sensor_vision DESTINATION lib/TinyCog)
install (FILES include/sense/vision/BoxTracker.hpp 
               include/sense/vision/BoxTrackerThread.hpp
	       include/sense/vision/CamCapture.hpp 
	       include/sense/vision/FileCapture.hpp 
	       include/sense/vision/ImageSource.hpp
	       include/sense/vision/ImageTransform.hpp 
	       include/sense/vision/ITColor2Gray.hpp 
	       include/sense/vision/ITEqualizeHist.hpp
	       include/sense/vision/ITDetectFace.hpp 
	       include/sense/vision/ITDetectSmile.hpp
	       include/sense/vision/DSaliency.hpp
	       include/sense/vision/FacialExpressions.hpp
	       include/sense/vision/FacialLandMark.hpp
	       include/sense/vision/FingersCount.hpp
	       include/sense/vision/ITDetectHand.hpp
	       include/sense/vision/RaspiCamCapture.hpp
	       include/sense/vision/RaspiVision.hpp
	       include/sense/vision/VisionCommon.hpp
	       include/sense/vision/Vision.hpp
    DESTINATION "include/TinyCog/sense/vision")


########AudioCapture
pkg_check_modules(ALSA alsa)
add_definitions(-DHAVE_AUDIO)
if(ALSA_FOUND)
	message(STATUS	"Alsa Found... Building Audio Capture Library")
	set(HAVE_AUDIO 1)
	add_library(sensor_audio ${CMAKE_SOURCE_DIR}/sense/audio/AudioCap.cpp)
	target_link_libraries(sensor_audio ${ALSA_LIBRARIES} pthread)

	# build example
	add_executable( AudioCapExample examples/sense/audio/AudioCapExample.cpp )
	target_link_libraries( AudioCapExample sensor_audio)
else(ALSA_FOUND)
	message(STATUS	"Alsa Not Found... Not Going to Build Audio Functionality")
	set(HAVE_AUDIO 0)
endif(ALSA_FOUND)


########STT
find_package( PocketSphinx REQUIRED )
if (POCKETSPHINX_FOUND)
    configure_file( ${CMAKE_SOURCE_DIR}/sense/audio/STT.cpp.in 
                   ${CMAKE_SOURCE_DIR}/sense/audio/STT.cpp @ONLY )
    include_directories("include" ${POCKETSPHINX_INCLUDE_DIRS})
    add_library( pstt sense/audio/STT.cpp )
    target_link_libraries( pstt ${POCKETSPHINX_LIBRARIES} ${EXTRA_LIBS} )

    add_executable( STT_Test examples/sense/audio/STT_Test.cpp )
    target_link_libraries (STT_Test pstt sensor_audio)

    install( TARGETS pstt DESTINATION lib/TinyCog)
    install( FILES include/sense/audio/STT.hpp DESTINATION "include/TinyCog/sense/audio")
else (POCKETSPHINX_FOUND)
    message("STT Will Not Be Built.")
endif (POCKETSPHINX_FOUND)


########TTS
find_package(Festival REQUIRED)
find_package(EST REQUIRED)
include_directories("include" "include/act/audio" ${FESTIVAL_INCLUDE_DIR} ${EST_INCLUDE_DIR})
add_library( act_audio act/audio/FestivalTTS.cpp )
set_property(TARGET act_audio PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries( act_audio ${FESTIVAL_LIBRARIES} ${EST_LIBRARIES})

add_executable( exampleTTS examples/act/audio/exampleTTS.cpp)
target_link_libraries( exampleTTS act_audio )

install (TARGETS act_audio DESTINATION lib/TinyCog)
install (FILES include/act/audio/TTS.hpp include/act/audio/FestivalTTS.hpp 
        DESTINATION "include/TinyCog/act/audio" )
        
#########network
include_directories("include" "include/network")
add_library( networkr network/StringServerUDP.cpp)
target_link_libraries( networkr ${EXTRA_LIBS} )

add_executable( exUDPserver examples/network/exUDPserver.cpp)
target_link_libraries(exUDPserver networkr)

install (TARGETS networkr DESTINATION lib/TinyCog)
install (FILES include/network/StringServerUDP.hpp DESTINATION "include/TinyCog/network")

#######COMM
find_package(WiringPi QUIET)
if(${WIRINGPI_FOUND})
include_directories("include/comm/spi" ${WIRINGPI_INCLUDE_DIR})
add_library(spicomm comm/spi/SpiComm.cpp)
target_link_libraries(spicomm ${WIRINGPI_LIB})

add_executable(SPICommTest examples/comm/spi/SPICommTest.cpp)
target_link_libraries(SPICommTest spicomm)

install( TARGETS spicomm DESTINATION lib/TinyCog )
install( FILES include/comm/spi/SpiComm.hpp DESTINATION "include/TinyCog/comm/spi")

endif(${WIRINGPI_FOUND})


######SCM Binding
if(raspicam_FOUND)
    set(ON_RPI_3 "YES")
    set(DR_ROBOTO_LIBS sensor_vision ${raspicam_CV_LIBS} 
                       ${EXTRA_LIBS} dlib::dlib act_audio)
else(raspicam_FOUND)
    set(ON_RPI_3 "NO")
    set(DR_ROBOTO_LIBS sensor_vision ${OpenCV_LIBS} 
                       ${EXTRA_LIBS} dlib::dlib act_audio)
endif(raspicam_FOUND)

configure_file(${CMAKE_SOURCE_DIR}/dr-roboto/dr-roboto.cpp.in 
               ${CMAKE_SOURCE_DIR}/dr-roboto/dr-roboto.cpp @ONLY)

add_library(dr_roboto SHARED dr-roboto/dr-roboto.cpp)
target_link_libraries(dr_roboto ${DR_ROBOTO_LIBS})
install( TARGETS dr_roboto DESTINATION lib/TinyCog )

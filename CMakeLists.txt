cmake_minimum_required(VERSION 3.1)
project( OpenCogER )
set (CMAKE_CXX_STANDARD 11)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(EXTRA_LIBS ${EXTRA_LIBS} pthread)
endif()
find_package( OpenCV REQUIRED )
include_directories("include")
add_library( sensor_vision  sense/vision/capture/CamCapture.cpp 
    sense/vision/capture/FileCapture.cpp 
    sense/vision/capture/ITColor2Gray.cpp 
    sense/vision/capture/ITEqualizeHist.cpp 
    sense/vision/detect/ITDetectFace.cpp 
    sense/vision/detect/ITDetectSmile.cpp 
    sense/vision/track/BoxTracker.cpp 
    sense/vision/track/BoxTrackerThread.cpp)
target_link_libraries( sensor_vision ${OpenCV_LIBS} ${EXTRA_LIBS})

add_executable(VisionTest examples/sense/vision/VisionTest.cpp)
target_link_libraries(VisionTest sensor_vision ${OpenCV_LIBS} ${EXTRA_LIBS})

add_executable(VisionTestFace examples/sense/vision/VisionTestFace.cpp)
target_link_libraries(VisionTestFace sensor_vision ${OpenCV_LIBS} ${EXTRA_LIBS})

install (TARGETS sensor_vision DESTINATION lib/OpenCogER)
install (FILES BoxTracker.hpp BoxTrackerThread.hpp
    CamCapture.hpp FileCapture.hpp ImageSource.hpp
    ImageTransform.hpp ITColor2Gray.hpp ITEqualizeHist.hpp
    ITDetectFace.hpp ITDetectSmile.hpp
    DESTINATION "include/OpenCogER/sense/vision")
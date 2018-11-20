############RPC
if (NOT DEFINED SERVER_ADDRESS)
  SET(SERVER_ADDRESS "localhost:50051")
endif()
configure_file( ${CMAKE_SOURCE_DIR}/include/comm/rpc/RPC_Client.hpp.in ${CMAKE_SOURCE_DIR}/include/comm/rpc/RPC_Client.hpp @ONLY)

set(PROTOF_PATH "${CMAKE_SOURCE_DIR}/protos")
set(GENERATED_PB_PATH "${CMAKE_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${GENERATED_PB_PATH})

function(PROTO_COMPILE PROTOF_NAME)
	#set(PROTOF_NAME "img_base")
	set(IMG_BASE_PROTOF "${PROTOF_PATH}/${PROTOF_NAME}.proto")
	set(IMG_BASE_PB_H_FILE "${GENERATED_PB_PATH}/${PROTOF_NAME}.pb.h")
	set(IMG_BASE_PB_CPP_FILE "${GENERATED_PB_PATH}/${PROTOF_NAME}.pb.cc")
	set(IMG_BASE_GRPC_PB_H_FILE "${GENERATED_PB_PATH}/${PROTOF_NAME}.grpc.pb.h")
	set(IMG_BASE_GRPC_PB_CPP_FILE "${GENERATED_PB_PATH}/${PROTOF_NAME}.grpc.pb.cc")

	add_custom_command(
		OUTPUT "${IMG_BASE_PB_H_FILE}"
		       "${IMG_BASE_PB_CPP_FILE}"
		       "${IMG_BASE_GRPC_PB_H_FILE}"
		       "${IMG_BASE_GRPC_PB_CPP_FILE}"
		COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
		ARGS "--proto_path=${PROTOF_PATH}"
		     "--cpp_out=${GENERATED_PB_PATH}"
		     "${IMG_BASE_PROTOF}"
		COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
		ARGS "--proto_path=${PROTOF_PATH}"
		     "--grpc_out=${GENERATED_PB_PATH}"
		     "--plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin"
		     "${IMG_BASE_PROTOF}"
	)

	set(GENERATED_PB_FILES ${IMG_BASE_PB_H_FILE} ${IMG_BASE_PB_CPP_FILE}
	                       ${IMG_BASE_GRPC_PB_H_FILE} ${IMG_BASE_GRPC_PB_CPP_FILE} PARENT_SCOPE)
endfunction(PROTO_COMPILE)

PROTO_COMPILE("img_base")
set(GENERATED_IB_PB_FILES ${GENERATED_PB_FILES})
PROTO_COMPILE("EmotionService")
set(GENERATED_FE_PB_FILES ${GENERATED_PB_FILES})

include_directories(${GENERATED_PB_PATH})

#BUILDING THE FACE BASIC SERVICE SERVER
add_library(rpc_ib_server ${CMAKE_SOURCE_DIR}/include/comm/rpc/RPC_Server_Image_Basic.hpp ${GENERATED_IB_PB_FILES})

#BUILDING THE FACE BASIC SERVICE
add_library(rpc_ib_client ${CMAKE_SOURCE_DIR}/comm/rpc/RPC_Client_Image_Basic.cpp ${GENERATED_IB_PB_FILES})

#BUILDING THE FACE EMOTION SERVICE
add_library(rpc_fe_client ${CMAKE_SOURCE_DIR}/comm/rpc/RPC_Client_Face_Emotion.cpp ${GENERATED_FE_PB_FILES})

if(raspicam_FOUND)
add_executable(RPC_Server_Image_Basic_Test ${CMAKE_SOURCE_DIR}/examples/comm/rpc/RPC_Server_Image_Basic_Test.cpp ${GENERATED_IB_PB_FILES})
target_link_libraries( RPC_Server_Image_Basic_Test rpc_ib_server grpc++_unsecure grpc gpr 
                                       ${PROTOBUF_LIBRARY} ${OpenCV_LIBS} sensor_vision)

add_executable(RPC_Client_Image_Basic_Test ${CMAKE_SOURCE_DIR}/examples/comm/rpc/RPC_Client_Test.cpp ${GENERATED_IB_PB_FILES})
target_link_libraries(RPC_Client_Face_Basic_Test rpc_ib_client grpc++_unsecure grpc gpr 
                     ${PROTOBUF_LIBRARY} ${raspicam_CV_LIBS} dlib::dlib sensor_vision)

#add_executable(RPC_Client_Face_Emotion_Test ${CMAKE_SOURCE_DIR}/examples/comm/rpc/RPC_Client_Test.cpp ${GENERATED_FE_PB_FILES})
#target_link_libraries(RPC_Client_Face_Basic_Test rpc_fe_client grpc++_unsecure grpc gpr 
#                     ${PROTOBUF_LIBRARY} ${raspicam_CV_LIBS} dlib::dlib sensor_vision base_64)
set(RPC_IB_LINK_LIBS rpc_ib_client grpc++_unsecure grpc gpr ${PROTOBUF_LIBRARY} ${raspicam_CV_LIBS} dlib::dlib sensor_vision PARENT_SCOPE)
#set(RPC_FE_LINK_LIBS rpc_fe_client grpc++_unsecure grpc gpr ${PROTOBUF_LIBRARY} ${raspicam_CV_LIBS} dlib::dlib sensor_vision base_64 PARENT_SCOPE)
endif(raspicam_FOUND)

cmake_minimum_required(VERSION 3.9)

# Qt5
find_package(Qt5 COMPONENTS Core Network REQUIRED)

#protobuf
#set yout protobuf dir on Windows
if (NOT UNIX AND NOT APPLE)
set(CMAKE_PREFIX_PATH
	"C:/protobuf-3.8.0"
	"${CMAKE_PREFIX_PATH}"
	)
endif()
find_package(Protobuf REQUIRED)
message(Protobuf_INCLUDE_DIRS " ${Protobuf_INCLUDE_DIRS}")
function(get_pb_file H CPP)
	list(APPEND PH ${ARGN})
	list(APPEND PC ${ARGN})
	list(TRANSFORM PH PREPEND ${CMAKE_BINARY_DIR}/proto/)
	list(TRANSFORM PH APPEND .pb.h)
	list(TRANSFORM PC PREPEND ${CMAKE_BINARY_DIR}/proto/)
	list(TRANSFORM PC APPEND .pb.cc)
	set(${H} ${PH} PARENT_SCOPE)
	set(${CPP} ${PC} PARENT_SCOPE)
endfunction()
SET(PROTO_META_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto)
file(MAKE_DIRECTORY ${PROTO_META_BASE_DIR})
LIST(APPEND PROTO_FLAGS -I${CMAKE_CURRENT_SOURCE_DIR}/proto)
file(GLOB PROTO_FILES "${CMAKE_SOURCE_DIR}/proto/*.proto")
# protobuf_generate_cpp(PROTO_CPP PROTO_H ${PROTO_FILES})
FOREACH(FIL ${PROTO_FILES})
	GET_FILENAME_COMPONENT(ABS_FIL ${FIL} ABSOLUTE)
	GET_FILENAME_COMPONENT(FIL_WE ${ABS_FIL} NAME_WE)
	# LIST(APPEND PROTO_CPP "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc")
	# LIST(APPEND PROTO_H "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h")
	EXECUTE_PROCESS(
		COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS} --cpp_out=${PROTO_META_BASE_DIR} ${FIL}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
ENDFOREACH()

#python
#set yout boost dir on Windows
if (NOT UNIX AND NOT APPLE)
  set(BOOST_ROOT C:/boost)
endif()
find_package(PythonInterp)
if (PYTHONINTERP_FOUND)
  if (UNIX AND NOT APPLE)
    if (PYTHON_VERSION_MAJOR EQUAL 3)
        find_package(Boost COMPONENTS python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR})
        find_package(PythonInterp 3)
        find_package(PythonLibs 3 REQUIRED)
    else()
        find_package(Boost COMPONENTS python)
        find_package(PythonInterp)
        find_package(PythonLibs REQUIRED)
    endif()
  else()	
    if (PYTHON_VERSION_MAJOR EQUAL 3)
        find_package(Boost COMPONENTS python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR})
        find_package(PythonInterp 3)
        find_package(PythonLibs 3 REQUIRED)
    else()
        find_package(Boost COMPONENTS python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR})
        find_package(PythonInterp)
        find_package(PythonLibs REQUIRED)
    endif()
  endif()
else()
    message("Python not found")
endif()

INCLUDE_DIRECTORIES(
	${Boost_INCLUDE_DIRS}
	${PYTHON_INCLUDE_DIRS}
    ${Protobuf_INCLUDE_DIRS}
	${Qt5Core_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_BINARY_DIR}/proto
)
LINK_LIBRARIES(${BOOST_LIBRARIES} 
	${PYTHON_LIBARIES} 
	${PROTOBUF_LIBRARIES} 
	Qt5::Core
	Qt5::Network)

set(PROTO_LIST
	vision_detection
)
get_pb_file(PROTO_H PROTO_CPP ${PROTO_LIST})

file(GLOB CPPFILES 
	"${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_library(visionIf SHARED
	pymodule.cc
	visionmodule.cpp
	${PROTO_CPP}
	${PROTO_H}
	${CPPFILES}
)

set_target_properties(visionIf PROPERTIES PREFIX "")

target_link_libraries(visionIf
	boost_python
    ${BOOST_LIBRARIES}
    ${PYTHON_LIBARIES}
)

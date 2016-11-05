cmake_minimum_required(VERSION 2.8)

project( Assignmen06 )

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "-std=c++0x")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

include_directories (
    include
)

add_executable( assignment06
		src/main.cpp
		src/agent.cpp
		src/environment.cpp )


cmake_minimum_required(VERSION 3.6)
project(amenability_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2" )

set(SOURCE_FILES main.cpp src/Graph.h src/Graph.cpp src/ColorRefinementAmenability.cpp src/ColorRefinementAmenability.h src/AuxiliaryMethods.h src/AuxiliaryMethods.cpp)
add_executable(amen ${SOURCE_FILES})
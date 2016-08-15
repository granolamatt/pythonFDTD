cmake_minimum_required(VERSION 2.8)
project(spam)

file(COPY main.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
set (CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")


add_library(spam SHARED spam.c ToyFDTD1.c)
target_link_libraries(spam ${PYTHON_LIBRARY})
set_target_properties(spam PROPERTIES PREFIX "" SUFFIX ".so")

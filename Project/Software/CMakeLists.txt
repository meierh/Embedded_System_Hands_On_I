cmake_minimum_required(VERSION 3.3)
project(HelloWorld)

FIND_PACKAGE(FLTK REQUIRED NO_MODULE)
include_directories(${FLTK_INCLUDE_DIRS}) 
link_directories(${FLTK_LIBRARY_DIRS}) 
add_definitions(${FLTK_DEFINITIONS})

add_executable(hello_world main.cpp HardwareEmulator.cpp System.cpp EmulatedSystem.cpp Application.cpp)

TARGET_LINK_LIBRARIES(hello_world fltk)

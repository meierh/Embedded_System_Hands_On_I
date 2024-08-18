cmake_minimum_required(VERSION 3.3)
project(HelloWorld)

FIND_PACKAGE(FLTK REQUIRED NO_MODULE)
include_directories(${FLTK_INCLUDE_DIRS}) 
link_directories(${FLTK_LIBRARY_DIRS}) 
add_definitions(${FLTK_DEFINITIONS})

file(GLOB ESHO1_SRC "*.cpp" "*.h")

add_executable(ESHO1 ${ESHO1_SRC})

TARGET_LINK_LIBRARIES(ESHO1 fltk)

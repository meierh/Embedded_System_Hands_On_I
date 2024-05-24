cmake_minimum_required (VERSION 2.8)
project (SWD)

add_executable (SWD main.cpp SWD.cpp)

include_directories(${CMAKE_SOURCE_DIR}/../../utils/pigpio)
link_directories(${CMAKE_SOURCE_DIR}/../../utils/pigpio/build)

set_target_properties(SWD PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(SWD pigpio)

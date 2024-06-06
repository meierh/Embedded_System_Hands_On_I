cmake_minimum_required (VERSION 2.8)
project (UART)

add_executable (UART_PI main.cpp UART.cpp)
include_directories(${CMAKE_SOURCE_DIR}/../../utils/pigpio)
link_directories(${CMAKE_SOURCE_DIR}/../../utils/pigpio/build)
set_target_properties(UART_PI PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(UART_PI pigpio)

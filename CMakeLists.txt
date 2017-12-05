cmake_minimum_required(VERSION 3.2)
project(FIFO_buffer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.c elem.h types.h config.h elem.c types.c buffer.h)
add_executable(FIFO_buffer ${SOURCE_FILES})

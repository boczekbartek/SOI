cmake_minimum_required(VERSION 3.2)
project(FIFO_buffer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.c src/elem.h src/types.h src/config.h src/elem.c src/types.c src/buffer.h)
add_executable(FIFO_buffer ${SOURCE_FILES})

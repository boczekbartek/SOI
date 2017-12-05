cmake_minimum_required(VERSION 3.2)
project(FIFO_buffer)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES src/main.c src/element.h src/types.h src/config.h src/element.c src/types.c src/buffer.h src/buffer.c src/producer.c src/producer.h)
add_executable(FIFO_buffer ${SOURCE_FILES})
set_property(TARGET FIFO_buffer PROPERTY C_STANDARD 99)


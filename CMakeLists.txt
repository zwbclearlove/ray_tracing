cmake_minimum_required(VERSION 3.16)

project(ray_tracing)
set(CMAKE_CXX_STANDARD 17)

include_directories(./include)
aux_source_directory(./src DIR_SRCS)

add_executable(ray_tracing ${DIR_SRCS}
        src/main.cpp)
cmake_minimum_required(VERSION 2.8)
project(work2)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(work2 work2.cpp)
target_link_libraries(work2 ${OpenCV_LIBS})

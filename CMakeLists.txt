cmake_minimum_required(VERSION 3.24)
project(Circuit3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Circuit3 main.cpp)
include_directories("C:/Program Files (x86)/Digilent/WaveFormsSDK/inc")
target_link_libraries(Circuit3 "C:/Program Files (x86)/Digilent/WaveFormsSDK/lib/x64/dwf.lib")
cmake_minimum_required(VERSION 3.7)

project(mc-gui)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(fonts)
add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(tests)

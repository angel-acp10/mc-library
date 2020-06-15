cmake_minimum_required(VERSION 3.7)

project(mc-gui)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(fonts)
add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(tests)

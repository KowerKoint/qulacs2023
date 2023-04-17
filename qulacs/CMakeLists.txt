cmake_minimum_required(VERSION 3.0)

add_library(qulacs)
add_subdirectory(internal)

target_sources(qulacs PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/state_vector.cpp
)
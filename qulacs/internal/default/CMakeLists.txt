cmake_minimum_required(VERSION 3.0)

target_sources(qulacs PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/init_ops_fill.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/init_ops_random.cpp
)
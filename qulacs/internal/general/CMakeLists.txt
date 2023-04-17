cmake_minimum_required(VERSION 3.0)

target_sources(qulacs PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/constant.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/omp_util.cpp
)
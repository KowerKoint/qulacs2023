cmake_minimum_required(VERSION 3.0)

file(GLOB_RECURSE SRC
    "*.cpp"
    "*.hpp"
)

add_library(qulacs_static STATIC ${SRC})
add_library(qulacs_shared SHARED ${SRC})
set_target_properties(qulacs_shared PROPERTIES EXCLUDE_FROM_ALL TRUE)
cmake_minimum_required(VERSION 3.0)
project(qulacs)

##### configure include files #####
file(GLOB header_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
foreach(header_path IN LISTS header_files)
	string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/src/ ${CMAKE_CURRENT_SOURCE_DIR}/include/qulacs/ header_path_dst ${header_path})
	configure_file(${header_path} ${header_path_dst} COPYONLY)
endforeach()

### setup output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

### Set C++ standard
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
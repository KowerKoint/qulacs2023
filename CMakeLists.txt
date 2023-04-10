cmake_minimum_required(VERSION 3.0)
project(qulacs)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)

##### configure include files #####
file(GLOB_RECURSE header_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
foreach(header_path IN LISTS header_files)
	string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/src/ ${CMAKE_CURRENT_SOURCE_DIR}/include/qulacs/ header_path_dst ${header_path})
	configure_file(${header_path} ${header_path_dst} COPYONLY)
endforeach()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
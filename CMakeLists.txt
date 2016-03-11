cmake_minimum_required(VERSION 2.6)
project(SGLTK)

set(SGLTK_VERSION_MAJOR 0)
set(SGLTK_VERSION_MINOR 2)

configure_file(
	"config.h.in"
	"${PROJECT_SOURCE_DIR}/config.h")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_subdirectory(src)
#add_subdirectory(doc)
#add_subdirectory(example)

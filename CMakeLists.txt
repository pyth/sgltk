cmake_minimum_required(VERSION 2.6)
project(sgltk)

set(SGLTK_VERSION_MAJOR 0)
set(SGLTK_VERSION_MINOR 2)

configure_file(
	"config.h.in"
	"${PROJECT_SOURCE_DIR}/config.h")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

#uninstall target
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

add_subdirectory(src)
add_subdirectory(doc)
#add_subdirectory(example)

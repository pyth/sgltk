cmake_minimum_required(VERSION 2.6)
project(sgltk)

set(SGLTK_VERSION_MAJOR 0)
set(SGLTK_VERSION_MINOR 2)
set(SGLTK_VERSION ${SGLTK_VERSION_MAJOR}.${SGLTK_VERSION_MINOR})

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for the library files")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for .dll files")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
	set(DEF_INSTALL_CMAKE_DIR cmake)
else()
	set(DEF_INSTALL_CMAKE_DIR lib/cmake/sgltk)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
	"Installation directory for CMake files")

foreach(p LIB BIN INCLUDE CMAKE)
	set(var ${INSTALL_${p}_DIR})
	if(NOT IS_ABSOLUTE "${${var}}")
		set(${var} "${CMAKE_INSTALL_PREFIX}/${var}")
	endif()
endforeach()

#find_package(GLEW REQUIRED)
#find_package(SDL2 REQUIRED)

configure_file("config.h.in"
	"${PROJECT_SOURCE_DIR}/src/config.h")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(example)

export(TARGETS sgltk sgltk_static
	FILE "${PROJECT_BINARY_DIR}/sgltkTargets.cmake")

export(PACKAGE sgltk)

file(RELATIVE_PATH REL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/${INSTALL_CMAKE_DIR}"
	"${CMAKE_INSTALL_PREFIX}/${INSTALL_INCLUDE_DIR}")
#set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
#configure_file(sgltkConfig.cmake.in "${PROJECT_BINARY_DIR}/sgltkConfig.cmake" @ONLY)
set(CONF_INCLUDE_DIRS "\${SGLTK_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(sgltkConfig.cmake.in
	"${PROJECT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/sgltkConfig.cmake" @ONLY)
configure_file(sgltkConfigVersion.cmake.in
	"${PROJECT_BINARY_DIR}/sgltkConfigVersion.cmake" @ONLY)

install(FILES
	"${PROJECT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/sgltkConfig.cmake"
	"${PROJECT_BINARY_DIR}/sgltkConfigVersion.cmake"
	DESTINATION "${INSTALL_CMAKE_DIR}")

install(EXPORT sgltkTargets
	DESTINATION "${INSTALL_CMAKE_DIR}")

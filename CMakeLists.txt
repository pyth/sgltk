if(WIN32)
	cmake_minimum_required(VERSION 3.4)
else()
	cmake_minimum_required(VERSION 2.8.4)
endif()
project(sgltk)

include(CheckCXXCompilerFlag)

set(SGLTK_VERSION_MAJOR 0)
set(SGLTK_VERSION_MINOR 5)
set(SGLTK_VERSION ${SGLTK_VERSION_MAJOR}.${SGLTK_VERSION_MINOR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(LIBRARY_PATH "" CACHE PATH "3rd party libraries")
set(INCLUDE_PATH "" CACHE PATH "3rd party library headers")
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${LIBRARY_PATH};${INCLUDE_PATH}")

include_directories(${INCLUDE_PATH})
link_directories(${LIBRARY_PATH})

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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR}/lib)
endforeach()

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

add_subdirectory(src)
add_subdirectory(doc)

export(TARGETS sgltk sgltk_static
	FILE "${PROJECT_BINARY_DIR}/sgltkTargets.cmake")

export(PACKAGE sgltk)

file(RELATIVE_PATH REL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/${INSTALL_CMAKE_DIR}"
	"${CMAKE_INSTALL_PREFIX}/${INSTALL_INCLUDE_DIR}")
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

if(UNIX)
	file(WRITE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.conf "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}")

	install(FILES
		"${PROJECT_BINARY_DIR}/${PROJECT_NAME}.conf"
		DESTINATION "/etc/ld.so.conf.d/")

	install(CODE "execute_process(COMMAND ldconfig)")
endif()

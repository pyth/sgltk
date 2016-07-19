set(LIB_SOURCE
	app.cpp
	window.cpp
	gamepad.cpp
	camera.cpp
	image.cpp
	texture.cpp
	scene.cpp
	shader.cpp
	timer.cpp
	mesh.cpp)

set(LIB_HEADERS
	config.h
	app.h
	window.h
	gamepad.h
	camera.h
	image.h
	texture.h
	scene.h
	shader.h
	timer.h
	mesh.h)

set(LIBS_REQUIRED SDL2 SDL2_image assimp)
set(LIBS_OPTIONAL SDL2_ttf)
if(WIN32)
	set(LIBS_REQUIRED OpenGL32 glew32 ${LIBS_REQUIRED})
else()
	set(LIBS_REQUIRED GL GLEW ${LIBS_REQUIRED})
endif()

find_file(TMP GL/glew.h)
if(TMP)
	set(HAVE_GLEW_H 1)
endif()
find_file(TMP SDL2/SDL.h)
if(TMP)
	set(HAVE_SDL_H 1)
endif()
find_file(TMP SDL2/SDL_ttf.h)
if(TMP)
	set(HAVE_SDL_TTF_H 1)
endif()
find_file(TMP SDL2/SDL_image.h)
if(TMP)
	set(HAVE_SDL_IMAGE_H 1)
endif()
find_file(TMP glm/glm.hpp)
if(TMP)
	set(HAVE_GLM_H 1)
endif()
find_file(TMP assimp/Importer.hpp)
if(TMP)
	set(HAVE_ASSIMP_H 1)
endif()

configure_file("${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_SOURCE_DIR}/src/config.h")

foreach(lib ${LIBS_REQUIRED})
	find_library(${lib}_FOUND ${lib} REQUIRED)
	if(${lib}_FOUND)
		message(STATUS "Library ${lib} - found")
	else()
		message(STATUS "Library ${lib} - not found")
	endif()
endforeach()

foreach(lib ${LIBS_OPTIONAL})
	find_library(${lib}_FOUND ${lib})
	if(${lib}_FOUND)
		message(STATUS "Library ${lib} - found")
	else()
		message(STATUS "Library ${lib} - not found")
	endif()
endforeach()

add_library(sgltk		SHARED ${LIB_SOURCE} ${LIB_HEADERS})
add_library(sgltk_static	STATIC ${LIB_SOURCE} ${LIB_HEADERS})

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
	set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

file(GLOB HEADERS "${PROJECT_BINARY_DIR}/src/*.h")
add_custom_target(copy_headers)
add_custom_command(TARGET copy_headers PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory
		${PROJECT_BINARY_DIR}/include/sgltk)
foreach(HEADER_FILE ${HEADERS})
	add_custom_command(TARGET copy_headers PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
			${HEADER_FILE}
			${PROJECT_BINARY_DIR}/include/sgltk/)
endforeach()
add_dependencies(sgltk copy_headers)
add_dependencies(sgltk_static copy_headers)

foreach(lib ${LIBS_REQUIRED} ${LIBS_OPTIONAL})
	if(${lib}_FOUND)
		target_link_libraries(sgltk ${lib})
		target_link_libraries(sgltk_static ${lib})
	endif()
endforeach()

string(REGEX REPLACE "\n" ";" HEADERS "${LIB_HEADERS}")
set_target_properties(sgltk PROPERTIES
	PUBLIC_HEADER "${HEADERS}")

install(TARGETS sgltk sgltk_static
	EXPORT sgltkTargets
	LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
	ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
	RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
	PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/${PROJECT_NAME}")

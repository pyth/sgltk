set(LIB_SOURCE
	app.cpp
	window.cpp
	gamepad.cpp
	joystick.cpp
	particle.cpp
	framebuffer.cpp
	renderbuffer.cpp
	camera.cpp
	camera_p.cpp
	camera_o.cpp
	camera_ip.cpp
	image.cpp
	texture.cpp
	cubemap.cpp
	texture_1d_array.cpp
	texture_1d.cpp
	texture_2d_array.cpp
	texture_2d.cpp
	texture_3d.cpp
	model.cpp
	shader.cpp
	timer.cpp
	mesh.cpp)

set(LIB_HEADERS
	config.h
	app.h
	window.h
	gamepad.h
	joystick.h
	particle.h
	framebuffer.h
	renderbuffer.h
	camera.h
	image.h
	texture.h
	model.h
	shader.h
	timer.h
	buffer.h
	mesh.h)

set(LIBS_REQUIRED SDL2 SDL2_image)
set(LIBS_OPTIONAL SDL2_ttf assimp)
if(WIN32)
	set(LIBS_REQUIRED OpenGL32 glew32 ${LIBS_REQUIRED})
else()
	set(LIBS_REQUIRED GL GLEW ${LIBS_REQUIRED})
endif()

find_file(GLEW_PATH NAMES GL/glew.h)
if(GLEW_PATH)
	set(HAVE_GLEW_H 1)
endif()

find_path(SDL2_PATH NAMES SDL2/SDL.h)
if(SDL2_PATH)
	include_directories("${SDL2_PATH}/SDL2")
	set(HAVE_SDL_H 1)
	set(SDL_ALT_PATH 1)
else()
	find_file(SDL2_PATH NAMES SDL.h)
	if(SDL2_PATH)
		set(HAVE_SDL_H 1)
	endif()
endif()

find_path(SDL2_TTF_PATH NAMES SDL2/SDL_ttf.h)
if(SDL2_TTF_PATH)
	include_directories("${SDL2_PATH}/SDL2")
	set(HAVE_SDL_TTF_H 1)
	set(SDL_ALT_PATH 1)
else()
	find_file(SDL2_TTF_PATH NAMES SDL_ttf.h)
	if(SDL2_TTF_PATH)
		set(HAVE_SDL_TTF_H 1)
	endif()
endif()

find_path(SDL2_IMAGE_PATH NAMES SDL2/SDL_image.h)
if(SDL_IMAGE_PATH)
	include_directories("${SDL2_PATH}/SDL2")
	set(HAVE_SDL_IMAGE_H 1)
	set(SDL_ALT_PATH 1)
else()
	find_file(SDL2_IMAGE_PATH NAMES SDL_image.h)
	if(SDL2_IMAGE_PATH)
		set(HAVE_SDL_IMAGE_H 1)
	endif()
endif()

find_file(GLM_PATH NAMES glm/glm.hpp)
if(GLM_PATH)
	set(HAVE_GLM_H 1)
endif()

find_file(ASSIMP_PATH NAMES assimp/Importer.hpp)
if(ASSIMP_PATH)
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

target_compile_definitions(sgltk PRIVATE MAKE_LIB)
target_compile_definitions(sgltk_static PRIVATE MAKE_LIB)
if(WIN32)
	target_compile_definitions(sgltk PRIVATE MAKE_DLL)
	target_compile_definitions(sgltk_static PRIVATE MAKE_STATIC)
endif()

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
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
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

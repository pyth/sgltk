set(LIB_SOURCE
	core.cpp
	app.cpp
	camera.cpp
	image.cpp
	texture.cpp
	scene.cpp
	shader.cpp
	timer.cpp)

set(LIB_HEADERS
	config.h
	core.h
	app.h
	camera.h
	image.h
	texture.h
	scene.h
	shader.h
	timer.h
	mesh.h)

set(LIBS_REQUIRED GL GLEW SDL2 SDL2_image assimp)
set(LIBS_OPTIONAL SDL2_ttf SDL2_mixer)

check_include_files(GL/glew.h			HAVE_GLEW_H)
check_include_files(SDL2/SDL.h			HAVE_SDL_H)
check_include_files(SDL2/SDL_ttf.h		HAVE_SDL_TTF_H)
check_include_files(SDL2/SDL_mixer.h		HAVE_SDL_MIXER_H)
check_include_files(SDL2/SDL_image.h		HAVE_SDL_IMAGE_H)
check_include_file_cxx(glm/glm.hpp		HAVE_GLM_H)
check_include_file_cxx(assimp/Importer.hpp	HAVE_ASSIMP_H)

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

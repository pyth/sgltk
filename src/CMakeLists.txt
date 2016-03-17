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
	core.h
	app.h
	camera.h
	image.h
	texture.h
	scene.h
	shader.h
	timer.h
	mesh.h)

set(LIBS GL GLU GLEW SDL2 SDL2_ttf SDL2_mixer SDL2_image assimp)

foreach(lib ${LIBS})
	find_library(${lib}_FOUND ${lib})
	if(${lib}_FOUND)
		message(STATUS "FOUND ${lib}")
	else()
		message(FATAL_ERROR "NOT FOUND ${lib}")
	endif()
endforeach()

add_library(sgltk		SHARED ${LIB_SOURCE} ${LIB_HEADERS})
add_library(sgltk_static	STATIC ${LIB_SOURCE} ${LIB_HEADERS})

foreach(lib ${LIBS})
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
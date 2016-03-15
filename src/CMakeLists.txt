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


#set(SGLTK_LIBRARIES
#	GLEW)

add_library(sgltk		SHARED ${LIB_SOURCE} ${LIB_HEADERS})
add_library(sgltk_static	STATIC ${LIB_SOURCE} ${LIB_HEADERS})

string(REGEX REPLACE "\n" ";" HEADERS "${LIB_HEADERS}")
set_target_properties(sgltk PROPERTIES
	PUBLIC_HEADER "${HEADERS}")

#target_link_libraries(sgltk ${LIBRARY_LIST})

install(TARGETS sgltk sgltk_static
	EXPORT sgltkTargets
	LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
	ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
	RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
	PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/${PROJECT_NAME}")

set(LIB_SOURCE
	core.cpp
	app.cpp
	camera.cpp
	image.cpp texture.cpp
	scene.cpp
	shader.cpp
	timer.cpp)

set(LIB_HEADERS
	core.h
	app.h
	camera.h
	texture.h
	scene.h
	shader.h
	timer.h
	mesh.h)

add_library(sgltk		SHARED ${LIB_SOURCE} ${LIB_HEADERS})
add_library(sgltk_static	STATIC ${LIB_SOURCE} ${LIB_HEADERS})

install(TARGETS sgltk_static
	ARCHIVE DESTINATION lib)

install(TARGETS sgltk
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin)

install(FILES ${LIB_HEADERS}
	DESTINATION include/${PROJECT_NAME})

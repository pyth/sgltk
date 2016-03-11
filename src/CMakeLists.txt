set(LIB_SOURCE
	core.cpp core.h
	app.cpp app.h
	camera.cpp camera.h
	image.cpp texture.cpp texture.h
	scene.cpp scene.h
	shader.cpp shader.h
	timer.cpp timer.h
	mesh.h)

add_library(sgltk_static	STATIC ${LIB_SOURCE})
add_library(sgltk		SHARED ${LIB_SOURCE})

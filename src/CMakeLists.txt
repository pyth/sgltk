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
	mesh.cpp
)

set(LIB_HEADERS
	${PROJECT_SOURCE_DIR}/include/sgltk/sgltk.h
	${PROJECT_SOURCE_DIR}/include/sgltk/config.h
	${PROJECT_SOURCE_DIR}/include/sgltk/app.h
	${PROJECT_SOURCE_DIR}/include/sgltk/window.h
	${PROJECT_SOURCE_DIR}/include/sgltk/gamepad.h
	${PROJECT_SOURCE_DIR}/include/sgltk/joystick.h
	${PROJECT_SOURCE_DIR}/include/sgltk/particle.h
	${PROJECT_SOURCE_DIR}/include/sgltk/framebuffer.h
	${PROJECT_SOURCE_DIR}/include/sgltk/renderbuffer.h
	${PROJECT_SOURCE_DIR}/include/sgltk/camera.h
	${PROJECT_SOURCE_DIR}/include/sgltk/image.h
	${PROJECT_SOURCE_DIR}/include/sgltk/texture.h
	${PROJECT_SOURCE_DIR}/include/sgltk/model.h
	${PROJECT_SOURCE_DIR}/include/sgltk/shader.h
	${PROJECT_SOURCE_DIR}/include/sgltk/timer.h
	${PROJECT_SOURCE_DIR}/include/sgltk/buffer.h
	${PROJECT_SOURCE_DIR}/include/sgltk/mesh.h
)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)
find_package(glm REQUIRED CONFIG)
find_package(assimp REQUIRED CONFIG)

configure_file("${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_SOURCE_DIR}/include/sgltk/config.h")

add_library(sgltk SHARED ${LIB_SOURCE})
add_library(sgltk_static STATIC ${LIB_SOURCE})

target_compile_definitions(sgltk PRIVATE MAKE_LIB)
target_compile_definitions(sgltk_static PRIVATE MAKE_LIB)
if(WIN32)
	target_compile_definitions(sgltk PRIVATE MAKE_DLL)
	target_compile_definitions(sgltk_static PRIVATE MAKE_STATIC)
endif()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
	set_target_properties(sgltk PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

target_include_directories(sgltk PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
	${SGLTK_INCLUDE_DIRS}
	${INCLUDE_PATH}
)
target_include_directories(sgltk_static PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
	${SGLTK_INCLUDE_DIRS}
	${INCLUDE_PATH}
)

target_link_libraries(sgltk PRIVATE
	glm::glm GLEW::GLEW SDL2_image::SDL2_image sdl_ttf::sdl_ttf SDL2::SDL2main assimp::assimp
)

target_link_libraries(sgltk_static PRIVATE
	glm::glm GLEW::GLEW SDL2_image::SDL2_image sdl_ttf::sdl_ttf SDL2::SDL2main assimp::assimp
)

set_target_properties(sgltk PROPERTIES
	PUBLIC_HEADER "${LIB_HEADERS}")
set_target_properties(sgltk_static PROPERTIES
	PUBLIC_HEADER "${LIB_HEADERS}")

install(TARGETS sgltk sgltk_static
	EXPORT sgltkTargets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
	PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/sgltk"
)

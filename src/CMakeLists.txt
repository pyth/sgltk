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
	list(APPEND SGLTK_INCLUDE_DIRS "${SDL2_PATH}/SDL2")
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
	list(APPEND SGLTK_INCLUDE_DIRS "${SDL2_PATH}/SDL2")
	set(HAVE_SDL_TTF_H 1)
	set(SDL_ALT_PATH 1)
else()
	find_file(SDL2_TTF_PATH NAMES SDL_ttf.h)
	if(SDL2_TTF_PATH)
		set(HAVE_SDL_TTF_H 1)
	endif()
endif()

find_path(SDL2_IMAGE_PATH NAMES SDL2/SDL_image.h)
if(SDL2_IMAGE_PATH)
	list(APPEND SGLTK_INCLUDE_DIRS "${SDL2_PATH}/SDL2")
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
	"${PROJECT_SOURCE_DIR}/include/sgltk/config.h")

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

foreach(lib ${LIBS_REQUIRED} ${LIBS_OPTIONAL})
	if(NOT ${lib}_FOUND MATCHES "-NOTFOUND$")
		target_link_libraries(sgltk PRIVATE ${${lib}_FOUND})
		target_link_libraries(sgltk_static PRIVATE ${${lib}_FOUND})
	endif()
endforeach()

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

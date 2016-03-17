set(BIN_SOURCE main.cpp gui.cpp)
set(BIN_HEADERS gui.h)

find_library(SGLTK_FOUND sgltk)
if(${SGLTK_FOUND})
	message(STATUS "FOUND sgltk")
endif()

add_executable(example ${BIN_SOURCE} ${BIN_HEADERS})

target_link_libraries(example sgltk)

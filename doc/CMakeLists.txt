find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install documentation (requires Dxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
	if(NOT DOXYGEN_FOUND)
		message(FATAL_ERROR "Doxygen is needed to build documentation.")
	endif()

	set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
	set(doxyfile ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

	configure_file(${doxyfile_in} ${doxyfile} @ONLY)

	add_custom_target(doc
		COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating documentation with Doxygen"
		VERBATIM)

#	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
#	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man DESTINATION ${MAN_INSTALL_DIR}/man3)
endif()

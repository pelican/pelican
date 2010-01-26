# - Run Doxygen
#
# Adds a doxygen target that runs doxygen to generate the html
# and optionally the LaTeX API documentation.
# The doxygen target is added to the doc target as dependency.
# i.e.: the API documentation is built with:
#  make doc
#
# USAGE: GLOBAL INSTALL
#
# Install it with:
#  cmake ./ && sudo make install
# Add the following to the CMakeLists.txt of your project:
#  include(UseDoxygen OPTIONAL)
# Optionally copy Doxyfile.in in the directory of CMakeLists.txt and edit it.
#
# USAGE: INCLUDE IN PROJECT
#
#  set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#  include(UseDoxygen)
# Add the Doxyfile.in and UseDoxygen.cmake files to the projects source directory.
#
#
# Variables you may define are:
#  DOXYFILE_OUTPUT_DIR - Path where the Doxygen output is stored. Defaults to "doc".
#
#  DOXYFILE_LATEX_DIR - Directory where the Doxygen LaTeX output is stored. Defaults to "latex".
#
#  DOXYFILE_HTML_DIR - Directory where the Doxygen html output is stored. Defaults to "html".
#

#
#  Copyright (c) 2009 Tobias Rautenkranz <tobias@rautenkranz.ch>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


macro(usedoxygen_set_default name value)
	if(NOT DEFINED "${name}")
		set("${name}" "${value}")
	endif()
endmacro()

#===============================================================================
macro(add_doxygen_target name doxyfile_name)
#message("")
#message("= Buidling Doxygen makefile for \"${name}\", doxyfile: \"${doxyfile_name}\".")
#message("")

find_package(Doxygen)

#file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doc")

if(DOXYGEN_FOUND)
    set(DOXYFILE_IN_${name})
    find_file(DOXYFILE_IN_${name} ${doxyfile_name}.in PATHS ${CMAKE_SOURCE_DIR}/cmake)
	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(${doxyfile_name}.in DEFAULT_MSG DOXYFILE_IN_${name})
endif()

if(DOXYGEN_FOUND AND DOXYFILE_IN_${name})
    set(DOXYFILE_OUTPUT_DIR)
	add_custom_target(doc_${doxyfile_name} ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/${doxyfile_name})
	usedoxygen_set_default(DOXYFILE_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doc/${name})
	usedoxygen_set_default(DOXYFILE_HTML_DIR "html")
	set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${DOXYFILE_OUTPUT_DIR}/${DOXYFILE_HTML_DIR}")

	set(DOXYFILE_LATEX "NO")
	set(DOXYFILE_PDFLATEX "NO")
	set(DOXYFILE_DOT "NO")

	find_package(LATEX)
	
	if(LATEX_COMPILER AND MAKEINDEX_COMPILER)
		set(DOXYFILE_LATEX "YES")
		usedoxygen_set_default(DOXYFILE_LATEX_DIR "latex")

		set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${DOXYFILE_OUTPUT_DIR}/${DOXYFILE_LATEX_DIR}")

		if(PDFLATEX_COMPILER)
			set(DOXYFILE_PDFLATEX "YES")
		endif()
		if(DOXYGEN_DOT_EXECUTABLE)
			set(DOXYFILE_DOT "YES")
		endif()

		add_custom_command(TARGET ${name}
			POST_BUILD
			COMMAND ${CMAKE_MAKE_PROGRAM}
			WORKING_DIRECTORY "${DOXYFILE_OUTPUT_DIR}/${DOXYFILE_LATEX_DIR}")
	endif()

	configure_file(${DOXYFILE_IN_${name}} ${doxyfile_name} ESCAPE_QUOTES IMMEDIATE @ONLY)

	get_target_property(DOC_TARGET doc TYPE)
	if(NOT DOC_TARGET)
		add_custom_target(doc)
	endif()
		
	add_dependencies(doc doc_${doxyfile_name})
	
endif()

endmacro()



# Usage:
#   INCLUDE_SETUP( Subdirectory fileList )
#
# Description:
#   This mactro creates an include directory with a standard config.h inside
#   For each file in the fileList a copy is made in BUILD_INCLUDE_DIR/Subdirectory
#   Dependency targets are set for SubDirectory/filename
#
SET(BUILD_INCLUDE_DIR ${CMAKE_BINARY_DIR}/include)
FILE(MAKE_DIRECTORY ${BUILD_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${BUILD_INCLUDE_DIR})

MACRO(INCLUDE_SETUP dest)
  FILE(MAKE_DIRECTORY ${BUILD_INCLUDE_DIR}/${dest})
  FOREACH(file ${ARGN})
      GET_FILENAME_COMPONENT(filename ${file} NAME )
      SET(in_file ${CMAKE_CURRENT_BINARY_DIR}/${dest}_${filename})
      SET(out_file ${BUILD_INCLUDE_DIR}/${dest}/${filename})
      FILE(WRITE ${in_file}
          "#include \"${CMAKE_CURRENT_SOURCE_DIR}/${file}\"\n"
          )
      CONFIGURE_FILE(${in_file} ${out_file} COPYONLY)
      INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${file} DESTINATION ${INCLUDE_INSTALL_DIR}/${dest} )
  ENDFOREACH(file)
ENDMACRO(INCLUDE_SETUP dest)

# -- generate the info for config.h
INCLUDE(CheckIncludeFiles)
INCLUDE(TestBigEndian)
TEST_BIG_ENDIAN(WORDS_BIGENDIAN)
CHECK_INCLUDE_FILES(malloc.h HAVE_MALLOC_H)
CHECK_INCLUDE_FILES("sys/param.h;sys/mount.h" HAVE_SYS_MOUNT_H)
CHECK_INCLUDE_FILES(stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILES(stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILES(strings.h HAVE_STRINGS_H)
CHECK_INCLUDE_FILES("string.h" HAVE_STRING_H)
CHECK_INCLUDE_FILES("sys/stat.h" HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILES("sys/types.h" HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILES( unistd.h HAVE_UNISTD_H)
CHECK_INCLUDE_FILES( memory.h HAVE_MEMORY_H)


#
# Load in Package Dependencies
#
MACRO(PACKAGE_DEPENDENCIES)
    # add specific packages
    set(config_header "${BUILD_INCLUDE_DIR}/config.h")
    set(config_in_file "${CMAKE_CURRENT_BINARY_DIR}/config.h.in")
    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/cmake/config.h.in "${config_in_file}")
    FOREACH(pack ${ARGN})
        #message( "Looking for Package ${pack}" )
        string(TOUPPER ${pack} packvar)
        FIND_PACKAGE(${pack} REQUIRED)
        IF(${packvar}_FOUND)
            FILE(APPEND ${config_in_file}
                "#define HAVE_${packvar} 1\n"
            )
        ENDIF(${packvar}_FOUND)
    ENDFOREACH(pack)
    CONFIGURE_FILE(${config_in_file} "${config_header}")
ENDMACRO( PACKAGE_DEPENDENCIES )





#
# Define a directory as a SUBPACKAGE( packageName subpackage_dependencies )
#
macro(SUBPACKAGE_ADD_LIBRARIES)

    if(SUBPACKAGE_CURRENT)
        list(INSERT SUBPACKAGE_LIBRARIES 0 ${ARGN})
        list(INSERT SUBPACKAGE_${SUBPACKAGE_CURRENT}_LIBS 0 ${ARGN})
        file(APPEND ${SUBPACKAGE_FILE}
            "LIST(INSERT SUBPACKAGE_${SUBPACKAGE_CURRENT}_LIBS 0 ${ARGN})\n")
    else(SUBPACKAGE_CURRENT)
        message(FATAL_ERROR
            "SUBPACKAGE_ADD_LIBRARIES specified outside of a SUBPACKAGE context")
    endif(SUBPACKAGE_CURRENT)

endmacro(SUBPACKAGE_ADD_LIBRARIES)


#
# private macro to generate the SUBPACKAGE_LIBRARIES variable
#
MACRO(SUBPACKAGE_GET_LIBS package)
    IF(NOT SUBPACKAGE_${package}_ADDED)
        FOREACH(pack ${SUBPACKAGE_${package}_DEPS} )
           SUBPACKAGE_GET_LIBS(${pack})
        ENDFOREACH(pack)
        IF(SUBPACKAGE_${package}_LIBS)
            LIST(INSERT SUBPACKAGE_LIBRARIES 0 ${SUBPACKAGE_${package}_LIBS})
        ENDIF(SUBPACKAGE_${package}_LIBS)
        SET(SUBPACKAGE_${package}_ADDED TRUE)
    ENDIF(NOT SUBPACKAGE_${package}_ADDED)
ENDMACRO(SUBPACKAGE_GET_LIBS)




# --- some utility macros
MACRO(ADD_SUFFIX rootlist suffix)
    SET(outlist)
    FOREACH(root ${${rootlist}})
        LIST(APPEND outlist ${root}${suffix})
    ENDFOREACH(root)
    SET(${rootlist} ${outlist})
ENDMACRO(ADD_SUFFIX)

MACRO(ADD_DIR_PREFIX prefix rootlist)
    SET(outlist)
    FOREACH(root ${${rootlist}})
        if(IS_ABSOLUTE ${root})
            LIST(APPEND outlist ${root})
        else(IS_ABSOLUTE ${root})
            LIST(APPEND outlist ${prefix}${root})
        endif(IS_ABSOLUTE ${root})
    ENDFOREACH(root)
    SET(${rootlist} ${outlist})
ENDMACRO(ADD_DIR_PREFIX)

#
# Macro to get the object files for a given target
#
MACRO( SUBPROJECT_OBJECT_FILES target outputObjectFiles )
    IF(SUBPACKAGE_CURRENT)
        # This hack inspired by the bug report : http://www.cmake.org/Bug/view.php?id=5155
        #
        # CMake generators are currently of 2 types: those which build single configurations, and those
        # which build multiple configurations. These 2 types use 2 different directory structures for where
        # they put their object files. The currently recommended way to deduce which type of generator
        # we're using, is to see if CMAKE_CONFIGURATION_TYPES is empty or not. If it's empty, then it's
        # single configuration. If it's non-empty, then it's multiple configuration, and contains a list of all
        # the configurations available. We're not interested in that list, only whether it's empty or non-empty.

        IF(CMAKE_CONFIGURATION_TYPES)
            # We have a multiple configuration generator. Use this directory structure.
            #
            # Note that CMAKE_BUILD_TYPE has no value when Visual Studio .sln files are generated.
            # This is because on MSVC, no build type is actually selected at generation time. The MSVC
            # user typically selects her build type after opening the .sln file. CMAKE_CFG_INTDIR expands
            # to a Visual Studio macro that will contain the right value, once Visual Studio is opened and
            # a build type is selected.
            SET(STATIC_OBJ_DIR
                ${CMAKE_CURRENT_BINARY_DIR}/${target}.dir/${CMAKE_CFG_INTDIR})
        ELSE(CMAKE_CONFIGURATION_TYPES)
            # We have a single configuration generator. Use this directory structure:
            SET(STATIC_OBJ_DIR
                ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${target}.dir)
        ENDIF(CMAKE_CONFIGURATION_TYPES)

        # Now we know what directory the objects live in. Construct the actual list of objects:
        # from the sources. We cannot glob as these files do not exist yet
        get_property( target_sources TARGET ${target} PROPERTY SOURCES )
        foreach( sourcefile ${target_sources} )
            if(IS_ABSOLUTE ${sourcefile})
                # absolutes will appear in the top level object dir
                GET_FILENAME_COMPONENT(source_name "${sourcefile}" NAME)
            else(IS_ABSOLUTE ${sourcefile})
                # relative will also be relative to the top level object dir
                set(source_name "${sourcefile}")
            endif(IS_ABSOLUTE ${sourcefile})
            LIST(APPEND ${outputObjectFiles} ${source_name}${CMAKE_C_OUTPUT_EXTENSION})
        endforeach( sourcefile )
        ADD_DIR_PREFIX(${STATIC_OBJ_DIR}/ ${outputObjectFiles})
        #MESSAGE("${SUBPACKAGE_CURRENT}: OBJECT=${${outputObjectFiles}}")
    ELSE(SUBPACKAGE_CURRENT)
        MESSAGE("Error: SUBPACKAGE_LIBRARY specified outside of a SUBPACKAGE context")
    ENDIF(SUBPACKAGE_CURRENT)
ENDMACRO( SUBPROJECT_OBJECT_FILES )

































#===============================================================================
# Macros for handling sub-packages
#===============================================================================

set(SUBPACKAGE_WORK_DIR ${CMAKE_BINARY_DIR}/_subpackages)
file(MAKE_DIRECTORY ${SUBPACKAGE_WORK_DIR})

mark_as_advanced(SUBPACKAGE_CURRENT)
mark_as_advanced(SUBPACKAGE_WORK_DIR)
mark_as_advanced(SUBPACKAGE_LIBRARIES)



#===============================================================================
# Macro:
#       LIBRARY_MODULE_REQUIREMENTS
#
# Description:
#       Set and gather the requirements of a single library module
#       and load its dependencies.
#
# Usage:
#       LIBRARY_MODULE_REQUIREMENTS([module library name] [single library name]
#
# Note:
#       This is used by the SUBPACKAGE and TEST_SUBPACKAGE macro
#===============================================================================
macro(LIBRARY_MODULE_REQUIREMENTS moduleName packageName)

    # Set the current subpackage name.
    set(SUBPACKAGE_CURRENT "${moduleName}")

    # Set the name of the global package library file to append to
    set(PACKAGE_LIBRARY_FILE "${SUBPACKAGE_WORK_DIR}/_${packageName}.cmake")

    # Set the name of the sub-package file/
    set(SUBPACKAGE_FILE "${SUBPACKAGE_WORK_DIR}/${moduleName}.cmake")

    # Install target for header files
    file(GLOB public_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")
    install(FILES ${public_headers}
        DESTINATION ${INCLUDE_INSTALL_DIR}/${moduleName})

    # Process packages that it depends on.
    set(SUBPACKAGE_${moduleName}_DEPS ${ARGN})
    if(SUBPACKAGE_${moduleName}_DEPS)
        list(REVERSE SUBPACKAGE_${moduleName}_DEPS)
    endif(SUBPACKAGE_${moduleName}_DEPS)

    file(WRITE ${SUBPACKAGE_FILE}
        "# Autogenerated file for the package : ${moduleName} - do not edit\n"
        "if(SUBPACKAGE_${moduleName}_LIBS)\n"
        "   return()\n"
        "endif(SUBPACKAGE_${moduleName}_LIBS)\n"
        "list(APPEND PROJECT_PACKAGES ${moduleName} ${ARGN})\n"
    )

    if(SUBPACKAGE_${moduleName}_DEPS)
        file(APPEND ${SUBPACKAGE_FILE}
            "set(SUBPACKAGE_${moduleName}_DEPS ${SUBPACKAGE_${moduleName}_DEPS})\n"
        )
    endif(SUBPACKAGE_${moduleName}_DEPS)

    # -- All include directories defined before the macro call are exported
    if(COMMAND GET_PROPERTY)
        get_property(includes DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    else(COMMAND GET_PROPERTY)
        # -- cmake 2.4 compatablity, just include everything
        set(includes ${CMAKE_INCLUDE_PATH})
    endif(COMMAND GET_PROPERTY)

    file(APPEND ${PACKAGE_LIBRARY_FILE}
            "list(INSERT ALL_SUBPACKAGES 0 ${SUBPACKAGE_CURRENT})\n"
            #"SUBPACKAGE_GET_LIBS(${moduleName})\n"
            #"include_directories(${SUBPACKAGE_CURRENT})\n"
    )

    foreach(inc ${includes})
        file(APPEND ${SUBPACKAGE_FILE} "include_directories(${inc})\n")
        file(APPEND ${PACKAGE_LIBRARY_FILE} "include_directories(${inc})\n")
    endforeach(inc)

    foreach(pack ${ARGN})
        file(APPEND ${SUBPACKAGE_FILE}
            "include(${SUBPACKAGE_WORK_DIR}/${pack}.cmake)\n")
    endforeach(pack)

    # Top level package directory for includes
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})

    foreach(dep ${SUBPACKAGE_${moduleName}_DEPS} )
        include(${SUBPACKAGE_WORK_DIR}/${dep}.cmake)
        SUBPACKAGE_GET_LIBS(${dep})
    endforeach(dep)

    if(SUBPACKAGE_LIBRARIES)
        #===========================================================================
        # CHANGE: uncommented (probably needs to be removed for static library)
        # list(REMOVE_DUPLICATES SUBPACKAGE_LIBRARIES)
        #===========================================================================
    endif(SUBPACKAGE_LIBRARIES)

    file(APPEND ${SUBPACKAGE_FILE}
        "if(PROJECT_PACKAGES)\n"
           "list(REMOVE_DUPLICATES PROJECT_PACKAGES)\n"
        "endif(PROJECT_PACKAGES)\n"
    )
endmacro(LIBRARY_MODULE_REQUIREMENTS)


#
# macro to generate requirements for a SUBPACKAGE
# and load dependencies
#
macro(SUBPACKAGE package)
   LIBRARY_MODULE_REQUIREMENTS(${package} global ${ARGN})
endmacro(SUBPACKAGE)

macro(TEST_SUBPACKAGE package)
   LIBRARY_MODULE_REQUIREMENTS(${package} test ${ARGN})
endmacro(TEST_SUBPACKAGE)




#===============================================================================
# Macro:
#       CREATE_LIBRARY_MODULE
#
# Description:
#       Macro to create a sub-library component of a single library.
#
#       This is required for a CMAKE project that builds a single library but
#       splits the project code into a number of sub-folders for which it also
#       required to build library with the code in that folder.
#
# Usage:
#       CREATE_MODULE_LIBRARY([module library name] [single library name]
#               [module library source])
#
# Note:
#       This macro is used by the SUBPACKAGE_LIBRARY and
#        TEST_SUBPACKAGE_LIBRARY macros.
#===============================================================================
macro(CREATE_LIBRARY_MODULE moduleName packageName)

    # If the module / sub-package is defined (set by LIBRARY_MODULE_REQUIREMENTS)
    if(SUBPACKAGE_CURRENT)

        # Add target for the dynamic library.
        add_library("${moduleName}" SHARED ${ARGN})

        # Add target for static library.
        if(BUILD_STATIC)
            add_library("${moduleName}_static" STATIC ${ARGN})
        endif(BUILD_STATIC)

        # Set properties for the library targets.
        # Prevent STATIC and SHARED trashing each others files.
        set_target_properties("${moduleName}" PROPERTIES CLEAN_DIRECT_OUTPUT 1)
        # Let both libs have the same name.
        if(BUILD_STATIC)
            set_target_properties("${moduleName}_static" PROPERTIES OUTPUT_NAME "${moduleName}")
            set_target_properties("${moduleName}_static" PROPERTIES PREFIX "lib")
            set_target_properties("${moduleName}_static" PROPERTIES CLEAN_DIRECT_OUTPUT 1)
        endif(BUILD_STATIC)

        # If building a single package library link against the other library
        # modules. [[ needed? ]]
        if(BUILD_SINGLE_LIB)
            if(PROJECT_LIBRARIES)
                SUBPACKAGE_ADD_LIBRARIES("${PROJECT_LIBRARIES}")
            else(PROJECT_LIBRARIES)
                message(FATAL_ERROR "ERROR: No PROJECT_LIBRARIES set")
            endif(PROJECT_LIBRARIES)
        else(BUILD_SINGLE_LIB)
            SUBPACKAGE_ADD_LIBRARIES("${moduleName}")
        endif(BUILD_SINGLE_LIB)

        # Set the name of the package library file build file
        set(PACKAGE_LIBRARY_FILE "${SUBPACKAGE_WORK_DIR}/_${packageName}.cmake")

        SUBPROJECT_OBJECT_FILES("${moduleName}" "${moduleName}_shared_objects")
        file(APPEND ${PACKAGE_LIBRARY_FILE}
            "list(INSERT SUBPACKAGE_SHARED_LIBRARIES 0 "${moduleName}")\n"
            "list(INSERT SUBPACKAGE_SHARED_OBJECTS 0 ${${moduleName}_shared_objects})\n"
        )
        if(BUILD_STATIC)
            SUBPROJECT_OBJECT_FILES("${moduleName}_static" "${moduleName}_static_objects")
            file(APPEND ${PACKAGE_LIBRARY_FILE}
                "list(INSERT SUBPACKAGE_STATIC_LIBRARIES 0 "${moduleName}_static")\n"
                "list(INSERT SUBPACKAGE_STATIC_OBJECTS 0 ${${moduleName}_static_objects})\n")
        endif(BUILD_STATIC)

        # Construct the global sources variable.
        foreach(sourcefile ${ARGN})
            if(IS_ABSOLUTE ${sourcefile})
                list(APPEND PROJECT_LIBRARY_SOURCES ${sourcefile})
            else(IS_ABSOLUTE ${sourcefile})
                list(APPEND PROJECT_LIBRARY_SOURCES ${SUBPACKAGE_CURRENT}/${sourcefile})
            endif(IS_ABSOLUTE ${sourcefile})
        endforeach(sourcefile)

        file(APPEND ${PACKAGE_LIBRARY_FILE}
            "list(APPEND PROJECT_LIBRARY_SOURCES ${PROJECT_LIBRARY_SOURCES})\n")

    else(SUBPACKAGE_CURRENT)
        message(FATAL_ERROR "ERROR: SUBPACKAGE_LIBRARY specified outside "
                            "of a SUBPACKAGE context")
    endif(SUBPACKAGE_CURRENT)

endmacro(CREATE_LIBRARY_MODULE)





#===============================================================================
# Macro for building subpackages of the main project.
#
# Appends to the build list for the global project library and builds
# the intermediate subpackge library.
#
#===============================================================================
macro(SUBPACKAGE_LIBRARY libname)


#        file(WRITE ${SUBPACKAGE_GLOBAL_FILE}
#            "# Autogenerated file for - do not edit\n"
#            "if(PACKAGE_GLOBAL)\n"
#            "   return()\n"
#            "endif(PACKAGE_GLOBAL)\n"
#        )

    CREATE_LIBRARY_MODULE(${libname} global ${ARGN})

endmacro(SUBPACKAGE_LIBRARY)


#===============================================================================
# Macro for building subpackages of the main project.
#
# Appends to the build list for the global project library and builds
# the intermediate subpackge library.
#
#===============================================================================
macro(TEST_SUBPACKAGE_LIBRARY libname)

#        file(WRITE ${SUBPACKAGE_GLOBAL_FILE}
#            "# Autogenerated file for - do not edit\n"
#            "if(PACKAGE_GLOBAL)\n"
#            "   return()\n"
#            "endif(PACKAGE_GLOBAL)\n"
#        )

    CREATE_LIBRARY_MODULE(${libname} test ${ARGN})

endmacro(TEST_SUBPACKAGE_LIBRARY)






#
#  Macro to create a target to build a single global library that includes
#  the contents of all the subpackage libraries (defined by SUBPACKAGE_LIBRARY)
#
macro(LINK_PROJECT_LIBRARY libname packageName)

    if(BUILD_SINGLE_LIB)

        set(SUBPACKAGE_GLOBAL_FILE "${SUBPACKAGE_WORK_DIR}/_${packageName}.cmake")
        include(${SUBPACKAGE_GLOBAL_FILE})

        # Mark the object files as generated to avoid a search/missing targets.
        foreach(obj ${SUBPACKAGE_SHARED_OBJECTS})
            set_source_files_properties(${obj} PROPERTIES GENERATED 1)
        endforeach(obj)

        if(BUILD_STATIC)
            foreach(obj ${SUBPACKAGE_STATIC_OBJECTS})
                set_source_files_properties(${obj} PROPERTIES GENERATED 1)
            endforeach(obj)
        endif(BUILD_STATIC)

        add_library("${libname}" SHARED ${SUBPACKAGE_SHARED_OBJECTS})

        if(BUILD_STATIC)
            add_library("${libname}_static" STATIC ${SUBPACKAGE_STATIC_OBJECTS})
            set_target_properties("${libname}_static" PROPERTIES OUTPUT_NAME "${libname}")
            set_target_properties("${libname}_static" PROPERTIES PREFIX "lib")
            set_target_properties("${libname}_static" PROPERTIES CLEAN_DIRECT_OUTPUT 1)
            set_target_properties("${libname}_static" PROPERTIES  LINKER_LANGUAGE CXX)
        endif(BUILD_STATIC)

        set_target_properties("${libname}" PROPERTIES LINKER_LANGUAGE CXX)

        add_dependencies("${libname}" ${SUBPACKAGE_SHARED_LIBRARIES})

        if(BUILD_STATIC)
            add_dependencies("${libname}_static" ${SUBPACKAGE_STATIC_LIBRARIES})
            install(TARGETS "${libname}_static" DESTINATION ${LIBRARY_INSTALL_DIR})
        endif(BUILD_STATIC)

        install(TARGETS "${libname}" DESTINATION ${LIBRARY_INSTALL_DIR})
        target_link_libraries("${libname}" ${PROJECT_LIBRARIES})

        # Hack inserted to add omp to the link table as seeminly it doesnt
        # pick it up from libmodules.so ...
        set_target_properties("${libname}" PROPERTIES
           COMPILE_FLAGS "${OpenMP_CXX_FLAGS}"
           LINK_FLAGS "${OpenMP_CXX_FLAGS}"
        )

        if(BUILD_STATIC)
            target_link_libraries("${libname}_static" ${PROJECT_LIBRARIES})
        endif(BUILD_STATIC)

        # REMOVE LATER: Debug printing...
        #=======================================================================
        #foreach(lib ${PROJECT_LIBRARIES})
        #    message(STATUS ".............. ${lib}")
        #endforeach()
        #=======================================================================

    endif(BUILD_SINGLE_LIB)
endmacro(LINK_PROJECT_LIBRARY)



macro(PROJECT_LIBRARY libName)
    LINK_PROJECT_LIBRARY(${libName} global)
endmacro(PROJECT_LIBRARY)

macro(TEST_LIBRARY libName)
    LINK_PROJECT_LIBRARY(${libName} test)
endmacro(TEST_LIBRARY)





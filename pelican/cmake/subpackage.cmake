#===============================================================================
# subpackage.cmake
#
# Macros:
#
# Variables:
#
#===============================================================================



#
# Macro to register and set / get sub-package dependencies.
#
macro(SUBPACKAGE name)

    # Set the current sub-package.
    set(subpackage_current "${name}")

    # Initilise the sub-package file.
    _INIT_SUBPACKAGE_FILE()

    # Set the project (single library) files to which the current sub-package belongs.
    _SET_SUBPACKAGE_PROJECT_FILES()

    # Add include_directories() defined before this macro to the sub-package file.
    _SET_INCLUDE_DIRECTORIES()

    #TODO TODO: need to iterate though the dependencies of dependant supacakges to
    # load their libs see _SUBPACKAGE_GET_LIBS()

    # Save and process packages that the sub-package depends on.
    _GET_SUBPACKAGE_DEPS(${ARGN})

    # Create the install target for header files of the sub-package.
    file(GLOB public_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")
    install(FILES ${public_headers} DESTINATION ${INCLUDE_INSTALL_DIR}/${name})

endmacro(SUBPACKAGE)



#
# Create a subpackage library.
#
macro(SUBPACKAGE_LIBRARY name)

    if(subpackage_current)

        # Add target for the dynamic library.
        add_library("${name}" SHARED ${ARGN})

        # TODO: link the library with externals....

        # Add list of object files created by the sub-pacakge to project files
        # using the sub-pacakge.  
        _SUBPROJECT_OBJECT_FILES("${name}" "${name}_shared_objects")
        foreach(project_file ${project_files})
            file(APPEND ${project_file}
                "list(INSERT shared_libs 0 "${name}")\n\n"
                "list(INSERT shared_objects 0 ${${name}_shared_objects})\n\n"
            )
        endforeach(project_file)

        # Add the library generated to list of sub-package libs in the sub-pacakge file.
        file(APPEND ${subpackage_file} 
            "list(INSERT ${subpackage_current}_shared_libs 0 "${name}")\n\n")

        # Add the library to the list of sub-pacakge libraries.
        # This is required for the case in which subpacakge binaries depend
        # locally on the subpackage library.
        _SET_SUBPACKAGE_LIBRARIES(${subpackage_current})

    else(subpackage_current)
        message(FATAL_ERROR "ERROR: SUBPACKAGE_LIBRARY "
            " specified outside of a SUBPACKAGE context")
    endif(subpackage_current)

endmacro(SUBPACKAGE_LIBRARY)


#
# Set the external libraries used by the sub-package
#
macro(SUBPACKAGE_SET_EXTERNAL_LIBRARIES)
    if(subpackage_current)
        list(APPEND ${SUBPACKAGE_LIBRARIES} ${ARGN})
        file(APPEND ${subpackage_file} 
            "set(${subpackage_current}_external_LIBS ${ARGN})\n")
        foreach(project_file ${project_files})
            file(APPEND ${project_file}
                "list(APPEND external_libs ${ARGN})\n\n")
        endforeach(project_file)
        
    else(subpackage_current)
        message(FATAL_ERROR "ERROR: SUBPACKAGE_SET_EXTERNAL_LIBRARIES "
            " specified outside of a SUBPACKAGE context")
    endif(subpackage_current)      
        
endmacro(SUBPACKAGE_SET_EXTERNAL_LIBRARIES)




# ==============================================================================
# Private utility macros
# ==============================================================================




#
# Get the library dependencies for the sub-package.
# In single library mode this will be a list of single (project) libraries
# otherwise it will be the list of sub-package libraries specified
# by the dependency list of the SUBPACKGE macro.
#
macro(_SET_SUBPACKAGE_LIBRARIES)
    
    set(subpackage_deps ${ARGN})

    # In single library mode work out which project libraries the sub-package
    # DEPS list belong to.
    if(BUILD_SINGLE_LIB)
        if(subpackage_deps)    
            foreach(dep ${subpackage_deps})
                #message(STATUS "++ dep: ${dep}\n")
                foreach(project_lib ${project_libraries})
                 #   message(STATUS "   ++ project lib: ${project_lib}\n")
                    foreach(subpackage ${${project_lib}_subpackage_DEPS})
                  #      message(STATUS "     ++ subpackage: ${subpackage}")
                        if("${dep}" STREQUAL "${subpackage}")
                            list(APPEND SUBPACKAGE_LIBRARIES ${project_lib})
                        endif("${dep}" STREQUAL "${subpackage}")
                    endforeach(subpackage)
                endforeach(project_lib)
            endforeach(dep)
        endif(subpackage_deps)
    else(BUILD_SINGLE_LIB)
        foreach(dep ${subpackage_deps})
            include(${SUBPACKAGE_WORK_DIR}/${dep}.cmake)
            list(APPEND SUBPACKAGE_LIBRARIES ${${dep}_shared_libs})
        endforeach(dep)
    endif(BUILD_SINGLE_LIB)

    # Grab external libraries ... ? NEEDED?
    foreach(dep ${subpackage_deps})
        include("${SUBPACKAGE_WORK_DIR}/${dep}.cmake")
        #message(STATUS "^^^ ${dep}_external_LIBS = ${${dep}_external_LIBS}")
        list(APPEND SUBPACKAGE_LIBRARIES ${${dep}_external_LIBS})
    endforeach(dep)

    list(REMOVE_DUPLICATES SUBPACKAGE_LIBRARIES)

    #message(STATUS "*** subpackage = ${subpackage_current}, deps = ${subpackage_deps}")
    #message(STATUS "*** SUBPACKAGE_LIBRARIES = '${SUBPACKAGE_LIBRARIES}'")
    
endmacro(_SET_SUBPACKAGE_LIBRARIES)




#
# Adds to the the SUBPACKAGE_LIBRARIES variable for libraries of subpacakge 
# dependencies.
#
macro(_SUBPACKAGE_GET_LIBS name)

    if(NOT subpackage_${name}_added)

        foreach(pack ${subpackage_${name}_DEPS})
            _SUBPACKAGE_GET_LIBS(${pack})
        endforeach(pack)
        
        if(subpackage_${name}_LIBS)
            list(INSERT SUBPACKAGE_LIBRARIES 0 ${subpackage_${name}_LIBS})
        endif(subpackage_${name}_LIBS)
        
        if(subpackage_${name}_external_LIBS)
            list(INSERT SUBPACKAGE_LIBRARIES 0 ${subpackage_${name}_external_LIBS})
        endif(subpackage_${name}_external_LIBS)
 
        set(subpackage_${name}_added TRUE)
        
    endif(NOT subpackage_${name}_added)
endmacro(_SUBPACKAGE_GET_LIBS)




#
# Extracts the object files for a sub-pacakge.
#
macro(_SUBPROJECT_OBJECT_FILES target outputObjectFiles)
    # This hack inspired by the bug report : http://www.cmake.org/Bug/view.php?id=5155
    #
    # CMake generators are currently of 2 types: those which build single configurations, and those
    # which build multiple configurations. These 2 types use 2 different directory structures for where
    # they put their object files. The currently recommended way to deduce which type of generator
    # we're using, is to see if CMAKE_CONFIGURATION_TYPES is empty or not. If it's empty, then it's
    # single configuration. If it's non-empty, then it's multiple configuration, and contains a list of all
    # the configurations available. We're not interested in that list, only whether it's empty or non-empty.

    if(CMAKE_CONFIGURATION_TYPES)
        # We have a multiple configuration generator. Use this directory structure.
        #
        # Note that CMAKE_BUILD_TYPE has no value when Visual Studio .sln files are generated.
        # This is because on MSVC, no build type is actually selected at generation time. The MSVC
        # user typically selects her build type after opening the .sln file. CMAKE_CFG_INTDIR expands
        # to a Visual Studio macro that will contain the right value, once Visual Studio is opened and
        # a build type is selected.
        set(STATIC_OBJ_DIR ${CMAKE_CURRENT_BINARY_DIR}/${target}.dir/${CMAKE_CFG_INTDIR})
    else(CMAKE_CONFIGURATION_TYPES)
        # We have a single configuration generator. Use this directory structure:
        set(STATIC_OBJ_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${target}.dir)
    endif(CMAKE_CONFIGURATION_TYPES)

    # Now we know what directory the objects live in. Construct the actual list of objects:
    # from the sources. We cannot glob as these files do not exist yet
    get_property( target_sources TARGET ${target} PROPERTY SOURCES )
    
    foreach( sourcefile ${target_sources} )
        if(IS_ABSOLUTE ${sourcefile})
            # absolutes will appear in the top level object dir
            get_filename_component(source_name "${sourcefile}" NAME)
        else(IS_ABSOLUTE ${sourcefile})
            # relative will also be relative to the top level object dir
            set(source_name "${sourcefile}")
        endif(IS_ABSOLUTE ${sourcefile})
        list(APPEND ${outputObjectFiles} ${source_name}${CMAKE_C_OUTPUT_EXTENSION})
    endforeach( sourcefile )
    
    _ADD_DIR_PREFIX(${STATIC_OBJ_DIR}/ ${outputObjectFiles})
endmacro(_SUBPROJECT_OBJECT_FILES )



macro(_ADD_DIR_PREFIX prefix rootlist)
    set(outlist)
    foreach(root ${${rootlist}})
        if(IS_ABSOLUTE ${root})
            list(APPEND outlist ${root})
        else(IS_ABSOLUTE ${root})
            list(APPEND outlist ${prefix}${root})
        endif(IS_ABSOLUTE ${root})
    endforeach(root)
    set(${rootlist} ${outlist})
endmacro(_ADD_DIR_PREFIX)


#
# Macro to initialise and a sub-package file.
#
macro(_INIT_SUBPACKAGE_FILE)
    # Initialise the sub-package file with an include guard to prevent double
    # loading.
    set(subpackage_file "${SUBPACKAGE_WORK_DIR}/${subpackage_current}.cmake")
    file(WRITE ${subpackage_file}
        "# Sub-package: '${subpackage_current}'\n"
        "#  This file is auto-generated by the build system - do not edit.\n"
        #"if(subpackage_${subpackage_current}_ADDED)\n"
        #"   return()\n"
        #"endif(subpackage_${subpackage_current}_ADDED)\n"
        #"set(subpackage_${subpackage_current}_ADDED true)\n\n"
    )
endmacro(_INIT_SUBPACKAGE_FILE)


#
# Set the project files for the current subpackage.
#
macro(_SET_SUBPACKAGE_PROJECT_FILES)
    # Work out which project libraries the sub-package belongs to and set
    # the project file names.
    set(project_libs "") 
    set(project_files "")
    foreach(project_lib ${project_libraries})
        foreach(subpackage ${${project_lib}_subpackage_DEPS})
            if("${subpackage}" STREQUAL "${subpackage_current}")
                #message(STATUS "**** subpackage ${name} found in project lib '${project_lib}', (${subpackage})")
                list(APPEND project_libs ${project_lib})
            endif("${subpackage}" STREQUAL "${subpackage_current}")
        endforeach(subpackage)
    endforeach(project_lib)
    foreach(project ${project_libs})
        list(APPEND project_files "${SUBPACKAGE_WORK_DIR}/_${project}.cmake")
    endforeach(project)        
endmacro(_SET_SUBPACKAGE_PROJECT_FILES)


#
# Add include directories for the subpacakge to the subpackage and project files.
#
macro(_SET_INCLUDE_DIRECTORIES)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}) # TODO: remove this when includes are fixed.
    if(COMMAND GET_PROPERTY)
        get_property(includes DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    else(COMMAND GET_PROPERTY)
        # cmake 2.4 compatibility, just include everthing.
        set(includes ${CMAKE_INCLUDE_PATH})
    endif(COMMAND GET_PROPERTY)
    list(REMOVE_DUPLICATES includes)
    file(APPEND ${subpackage_file} "include_directories(${includes})\n\n")
    foreach(project_file ${project_files})
        file(APPEND ${project_file} "######### subpackge: ${subpackage_current} ##########\n\n")
        file(APPEND ${project_file} "include_directories(${includes})\n\n")
    endforeach(project_file)
endmacro(_SET_INCLUDE_DIRECTORIES)



#
# Process subpackage dependencies.
#
# Called by SUBPACKAGE.
#
# - Sets SUBPACKAGE_LIBRARIES
#
macro(_GET_SUBPACKAGE_DEPS)
    
    # Set teh variable dependencies from the argument list of passed down
    # from the SUBPACKAGE macro.
    set(dependencies ${ARGN})

    # If any dependencies exist add them to the subpackage file.
    if(dependencies)
        # Reverse the list of dependencies. - WHY?
        list(REVERSE dependencies)
        # Add the list of dependencies to the sub-package file.
        file(APPEND ${subpackage_file}
            "set(${subpackage_current}_subpackage_DEPS ${dependencies})\n\n")
    endif(dependencies)
    
    # Get the list of subpackage libraries to link against.
    # In single library build mode this will be the project libraries
    # for which the module dependencies belong.
    set(SUBPACKAGE_LIBRARIES "")
    _SET_SUBPACKAGE_LIBRARIES(${dependencies})
    
    # TODO also pull in external libraries (only really needed in STATIC mode)
    # as libs should be linked properly in SHARED mode.
    
    file(APPEND ${subpackage_file} 
        "set(${subpackage_current}_SUBPACKAGE_LIBRARIES ${SUBPACKAGE_LIBRARIES})\n\n")
    
    # Add the package files of dependent sub-packages to the sub-package file. #TODO: needed?
    foreach(pack ${ARGN})
        file(APPEND ${subpackage_file} 
            "include(${SUBPACKAGE_WORK_DIR}/${pack}.cmake)\n")
        foreach(project_file ${project_files})
            file(APPEND ${project_file} 
                "include(${SUBPACKAGE_WORK_DIR}/${pack}.cmake)\n")
        endforeach(project_file)

    endforeach(pack)
    file(APPEND ${subpackage_file} "\n")
    
endmacro(_GET_SUBPACKAGE_DEPS)

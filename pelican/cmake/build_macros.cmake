#===============================================================================
# build_macros.cmake
#
# version 1.0, 3 Jan 2013
#===============================================================================
#
# Description:
#   Set of CMake macros for building modular projects.
#
# Defines the following macros:
#   - DECLARE_PACKAGE_LIBRARAY
#   - DECLARE_MODULE_LIBRARY
#   - CREATE_LIBRARY_TARGETS
#   - CREATE_HEADER_INSTALL_TARGET
#
# (utility macros)
#   - SET_BUILD_OPTION_DEFAULTS
#   - PRINT_BUILD_OPTIONS
#   - PRINT_MODULE_SUMMARY
#
# Note: This currently requires FindQt4 due to the qt4_wrap_cpp call
#       - this could probably be guarded to support build systems with no qt
#         dependency
#
# Build Options (variables) used:
#   - BUILD_SHARED       (ON/OFF)
#   - BUILD_STATIC       (ON/OFF)
#   - BUILD_MODULES      (ON/OFF)
#   - BUILD_PACKAGES     (ON/OFF)
#
# Example:
#   !!TODO!!
#
#===============================================================================

#-------------------------------------------------------------------------------
# Macro: DECLARE_PACKAGE_LIBRARY
#-------------------------------------------------------------------------------
#
# Description:
#   Declare a package library into the build system.
#
#   TODO ...
#      clarify what is a package library?!
#      clarify what is a module
#      Why is this macro needed... (details)?
#
# Usage:
#   DECLARE_PACKAGE_LIBRARAY(name modules(s))
#
# Arguments
#   - name       : The name of the package library
#   - modules(s) : A list of modules from which the package is built
#
# Example:
#   DECLARE_PACKAGE_LIBRARAY(myPackage module1 module2)
#
# Details:
#   This macro populates the variables:
#     DECLARED_PACKAGES ... Appends to this list of all currently declared
#                           packages
#     [package]_MODULES ... Modules required for building the package
#
macro(DECLARE_PACKAGE_LIBRARY name)
    if (NOT BUILD_SHARED AND NOT BUILD_STATIC)
        message(FATAL_ERROR
        "-------------------------\n"
        "ERROR: Please set at least one of BUILD_SHARED & BUILD_STATIC to ON."
        "\n-------------------------\n")
    endif ()
    if (NOT BUILD_PACKAGES AND NOT BUILD_MODULES)
        message(FATAL_ERROR
        "-------------------------\n"
        "ERROR: Please set at least one of BUILD_PACKAGES & BUILD_MODULES to ON."
        "\n-------------------------\n")
    endif ()
    if (NOT DECLARED_PACKAGES)
        set(DECLARED_PACKAGES ${name})
    else()
        list(APPEND DECLARED_PACKAGES ${name})
    endif()
    set(${name}_MODULES ${ARGN})
endmacro(DECLARE_PACKAGE_LIBRARY)


#-------------------------------------------------------------------------------
# Macro: DECLARE_MODULE_LIBRARY
#-------------------------------------------------------------------------------
#
# Description:
#   Declare/register a module into the build system.
#
# Usage:
#   DECLARE_MODULE_LIBRARY(
#       name
#       SRC   src1 src2 ...
#       [MOC  moc_src1 moc_src2 ...]
#       [DEPS module1 module2]
#       [LIBS lib1 lib2])
#
# Arguments:
#   name    Name of the module.
#   SRC     SRC followed by a list of source files required for the module.
#   MOC     MOC followed by a list of source files required for the module which
#           need to be processed with the Qt MOC complier. i.e. source files
#           contating classes with the Q_OBJECT declaration.
#   DEPS    DEPS followed by a list of modules that this module depedends on.
#   LIBS    LIBS followed by a list of external libraries that this module
#           depends on.
#
# Example:
#   DECLARE_MODULE_LIBRARY(
#       myModule
#       SRC     src/source1.cpp src/source2.cpp
#       MOC     myQObject.hpp
#       DEPS    anotherModule
#       LIBS    someExternalLib)
#
# Details:
#   This macro populates the variables:
#      [module]_PATH    ... Path of the module relative to the project source
#                           directory (top level CMakeLists.txt file)
#      [module]_SRC     ... Source files for the module.
#      [module]_MOC     ... Source files for the module requiring Qt MOC
#                           complication
#      [module]_DEPS    ... Module dependencies (on other modules)
#      [module]_LIBS    ... Module external library dependencies.
#      [module]_PACKAGE ... Package of the module
#      [module]_LIBRARY ... (library target of the current module)
#      DECLARED_MODULES ... Appends to this list which holds the list of all
#                           currently declared modules.
#
macro(DECLARE_MODULE_LIBRARY)
    parse_arguments(ARG           # prefix
        "SRC;DEPS;LIBS;MOC"       # arg names
        "NO_INSTALL;STATIC;SHARED" # option names
        ${ARGN})
    first_arg(ARG_NAME ${ARG_DEFAULT_ARGS})

    string(REGEX REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" "" path
        "${CMAKE_CURRENT_SOURCE_DIR}")
    set(module ${ARG_NAME})

    # TODO catch (with useful error message) for ${module} being empty...!

    list(APPEND DECLARED_MODULES ${module})
    set(DECLARED_MODULES ${DECLARED_MODULES} CACHE INTERNAL "" FORCE)

    # Evaluate the current package based on the declared packages.
    #message(">>>>>>> MODULE: ${module}")
    #message(">>>>>>>   DECLARED_PACKAGES: ${DECLARED_PACKAGES}")
    foreach (p ${DECLARED_PACKAGES})
        foreach (m ${${p}_MODULES})
            #message(">>>>>>>     ${p} -- ${m} ?")
            if (${module} STREQUAL ${m})
                #message(">>>>>>>       YES!")
                set(package ${p})
                break()
            endif()
        endforeach()
        #if (package)
        #    break()
        #endif()
    endforeach()
    if (NOT package)
        set(package "UNDEF")
    endif()

    # Export package variables to global scope.
    set(${module}_PATH ${path}       CACHE INTERNAL "" FORCE)
    set(${module}_SRC  ${ARG_SRC}    CACHE INTERNAL "" FORCE)
    set(${module}_MOC  ${ARG_MOC}    CACHE INTERNAL "" FORCE)
    set(${module}_DEPS ${ARG_DEPS}   CACHE INTERNAL "" FORCE)
    set(${module}_LIBS ${ARG_LIBS}   CACHE INTERNAL "" FORCE)
    set(${module}_PACKAGE ${package} CACHE INTERNAL "" FORCE)

    #
    # Construct [module]_LIBRARY variable.
    # This constains the library required to link binaries which depend on the
    # current module library.
    #
    # This variable should be used for linking instead of an explicit
    # link to the library name as in order to make use CMake's internal (and
    # very smart) dependency scanner the link should be with the CMake library
    # target name rather than the name of the library itself.
    # While in many cases the library target and library name will be the same,
    # in order to support all of the build options avaiable to these set of
    # build macros the library link target is one of:
    #
    #     - module
    #     - module_static
    #     - package
    #     - package_static
    #
    # The variable [module]_LIBRARY is populated with the library target
    # corresponding to the selected buid options.
    #
    if (BUILD_MODULES OR ${package} MATCHES "UNDEF")
        if (BUILD_SHARED)
            set(${module}_LIBRARY ${module})
        elseif (BUILD_STATIC)
            set(${module}_LIBRARY ${module}_static)
        endif()
    else() # BUILD_PACKAGES
        if (BUILD_SHARED)
            set(${module}_LIBRARY ${package})
        elseif (BUILD_STATIC)
            set(${module}_LIBRARY ${package}_static)
        endif ()
    endif()
    set(${module}_LIBRARY ${${module}_LIBRARY} CACHE INTERNAL "" FORCE)

    # Debuging (prints variables exported from this macro)
    set(DEBUG_PRINT OFF)
    if (DEBUG_PRINT)
        message("")
        message("${module}_PATH    = ${${module}_PATH}")
        message("${module}_SRC     = ${${module}_SRC}")
        message("${module}_DEPS    = ${${module}_DEPS}")
        message("${module}_LIBS    = ${${module}_LIBS}")
        message("${module}_PACKAGE = ${${module}_PACKAGE}")
        message("${module}_LIBRARY = ${${module}_LIBRARY}")
        message("")
    endif()

endmacro(DECLARE_MODULE_LIBRARY)


#-------------------------------------------------------------------------------
# Macro: CREATE_LIBRARY_TARGETS
#-------------------------------------------------------------------------------
#
# Description:
#   Creates targets (build and install for the) declared project packages and
#   modules.
#
# Usage:
#   CREATE_LIBRARY_TARGETS(LIBRARY_VERSION LIBRARY_INSTALL_DIR)
#
# Arguments
#   LIBRARY_VERSION ....... Version number for shared libraries.
#   LIBRARY_INSTALL_DIR ... Install directory for libraries.
#
# Details:
#   This macro constructs package and module library targets based on the
#   values of build options (BUILD_SHARED, BUILD_STATIC, BUILD_PACKAGES, and
#   BUILD_MODULES) for the packages and modules declared with the
#   DECLARE_PACKAGE_LIBRARAY() and DECLARE_MODULE_LIBRARAY() macros.
#
macro(CREATE_LIBRARY_TARGETS LIBRARY_VERSION LIBRARY_INSTALL_DIR)

    # Create targets for module libraries
    if (BUILD_MODULES)
        foreach(module ${DECLARED_MODULES})

            # Construct path to source file
            foreach (file ${${module}_SRC})
                list(APPEND "${module}_sources" "${${module}_PATH}/${file}")
            endforeach()

            # Shared module libraries.
            if (BUILD_SHARED)
                foreach (file ${${module}_MOC})
                    get_filename_component(filename ${file} NAME)
                    qt4_wrap_CPP(${module}_MOC_SRC "${${module}_PATH}/${filename}")
                endforeach()
                add_library("${module}" SHARED
                    ${${module}_sources}
                    ${${module}_MOC_SRC})
                target_link_libraries("${module}"
                    ${${module}_DEPS} ${${module}_LIBS})
                set_target_properties("${module}" PROPERTIES
                    CLEAN_DIRECT_OUTPUT 1
                    LIBRARY_OUTPUT_DIRECTORY "${${module}_PATH}"
                    SOVERSION ${LIBRARY_VERSION}
                    VERSION ${LIBRARY_VERSION})
                module_object_files("${module}" ${module} "${${module}_PACKAGE}_shared_objs")
                install(TARGETS ${module} DESTINATION ${LIBRARY_INSTALL_DIR})
            endif(BUILD_SHARED)

            # Static module libraries.
            if (BUILD_STATIC)
                foreach (file ${${module}_MOC})
                    get_filename_component(filename ${file} NAME)
                    qt4_wrap_CPP(${module}_MOC_SRC "${${module}_PATH}/${filename}")
                endforeach()
                add_library("${module}_static" STATIC
                    ${${module}_sources}
                    ${${module}_MOC_SRC})
                foreach (d ${${module}_DEPS})
                    target_link_libraries("${module}_static" ${d}_static)
                endforeach()
                target_link_libraries("${module}_static" ${${module}_LIBS})
                set_target_properties("${module}_static" PROPERTIES
                    CLEAN_DIRECT_OUTPUT 1 PREFIX ""
                    LIBRARY_OUTPUT_DIRECTORY "${${module}_PATH}"
                    OUTPUT_NAME "${${module}_PATH}/lib${module}")
                module_object_files("${module}_static" ${module} "${${module}_PACKAGE}_static_objs")
                install(TARGETS ${module}_static DESTINATION ${LIBRARY_INSTALL_DIR})
            endif(BUILD_STATIC)

        endforeach(module ${DECLARED_MODULES})

    endif(BUILD_MODULES)

    # Build the package library.
    if (BUILD_PACKAGES)

        foreach (package ${DECLARED_PACKAGES})

            # Construct package library variables, sources, libs, deps.
            foreach (module ${${package}_MODULES})
              # Construct path to source file
                foreach (file ${${module}_SRC})
                    list(APPEND "${module}_sources" "${${module}_PATH}/${file}")
                endforeach()
                foreach (file ${${module}_MOC})
                    get_filename_component(filename ${file} NAME)
                    qt4_wrap_CPP(${module}_MOC_SRC "${${module}_PATH}/${filename}")
                endforeach()
                list(APPEND "${package}_SRC"  ${${module}_sources})
                list(APPEND "${package}_LIBS" ${${module}_LIBS})
                list(APPEND "${package}_MOC_SRC" ${${module}_MOC_SRC})
                foreach (dep ${${module}_DEPS})
                    foreach (p ${DECLARED_PACKAGES})
                        foreach (m ${${p}_MODULES})
                            if (${dep} MATCHES ${m})
                                list(APPEND ${package}_DEPS ${p})
                            endif()
                        endforeach()
                    endforeach()
                endforeach()
            endforeach()

            if (${package}_LIBS)
               list(REMOVE_DUPLICATES ${package}_LIBS)
            endif()
            if (${package}_DEPS)
                list(REMOVE_ITEM ${package}_DEPS ${package})
                list(REMOVE_DUPLICATES ${package}_DEPS)
            endif()

            set(DEBUG_PRINT OFF)
            if (DEBUG_PRINT)
                message("")
                message("o Package: ${package}")
#                message("= SRC:")
#                foreach(s ${${package}_SRC})
#                    message("  ${s}")
#                endforeach()
#                message("= MOC_SRC:")
#                foreach(s ${${package}_MOC_SRC})
#                    message("  ${s}")
#                endforeach()
                set(n 0)
                list(LENGTH ${package}_LIBS n)
                if (${n} GREATER 0)
                    message("  = LIBS:")
                    foreach (p ${${package}_LIBS})
                        message("    ${p}")
                    endforeach()
                endif()
                set(n 0)
                list(LENGTH ${package}_DEPS n)
                if (${n} GREATER 0)
                    message("  = DEPS:")
                    foreach (p ${${package}_DEPS})
                        message("    ${p}")
                    endforeach()
                endif()
                message("")
            endif(DEBUG_PRINT)

            # Shared package library.
            if (BUILD_SHARED)
                # If we have already build shared modules, reuse object files
                if (BUILD_MODULES)
                    foreach (obj ${${package}_shared_objs})
                        set_source_files_properties(${obj} PROPERTIES
                            GENERATED 1)
                    endforeach()
                    add_library("${package}" SHARED
                        ${${package}_shared_objs})
                    set_target_properties("${package}" PROPERTIES
                        LINKER_LANGUAGE CXX)
                    add_dependencies("${package}" ${${package}_MODULES})
                else()
                    add_library("${package}" SHARED
                        ${${package}_SRC}
                        ${${package}_MOC_SRC})
                endif()
                target_link_libraries("${package}" ${${package}_LIBS} ${${package}_DEPS})
                set_target_properties("${package}" PROPERTIES
                    CLEAN_DIRECT_OUTPUT 1
                    SOVERSION ${LIBRARY_VERSION}
                    VERSION ${LIBRARY_VERSION})
                install(TARGETS ${package} DESTINATION ${LIBRARY_INSTALL_DIR})
            endif(BUILD_SHARED)

            # Static package library.
            if (BUILD_STATIC)
                # If we have already built the module libaries, reuse object files
                if (BUILD_MODULES)
                    foreach (obj ${${package}_static_objs})
                        set_source_files_properties(${obj} PROPERTIES GENERATED 1)
                    endforeach()
                    add_library("${package}_static" STATIC ${${package}_static_objs})
                    set_target_properties("${package}_static" PROPERTIES
                        LINKER_LANGUAGE CXX)
                    foreach (pm ${${package}_MODULES})
                        add_dependencies("${package}_static" ${pm}_static)
                    endforeach()
                else()
                    add_library("${package}_static" STATIC
                         ${${package}_SRC}
                         ${${package}_MOC_SRC})
                endif()
                foreach (pd ${${package}_DEPS})
                    target_link_libraries("${package}_static" ${pd}_static)
                endforeach()
                target_link_libraries("${package}_static" ${${package}_LIBS})
                set_target_properties("${package}_static" PROPERTIES
                    CLEAN_DIRECT_OUTPUT 1
                    PREFIX "lib"
                    OUTPUT_NAME "${package}")
                install(TARGETS ${package}_static DESTINATION ${LIBRARY_INSTALL_DIR})
            endif(BUILD_STATIC)

        endforeach(package ${DECLARED_PACKAGES})

        # Build any modules that dont have an assigned package.
        # FIXME the contents of BUILD_SHARED and BUILD_STATIC here is
        # this same as the modules section ... make functions...?
        if (NOT BUILD_MODULES)
            foreach (module ${DECLARED_MODULES})
                #message(">>>>>>>> ${module} ${${module}_PACKAGE}")
                if (${${module}_PACKAGE} MATCHES "UNDEF")

                    #message("  >>> ${module}")
                    # Construct path to source file
                    foreach (file ${${module}_SRC})
                        list(APPEND "${module}_sources" "${${module}_PATH}/${file}")
                    endforeach()

                    # Convert module DEPS to packages (as we are not building modules)
                    foreach(dep ${${module}_DEPS})
                        list(APPEND MODULE_PACKAGE_DEPS ${${dep}_PACKAGE})
                    endforeach()
                    list(REMOVE_DUPLICATES MODULE_PACKAGE_DEPS)

                    #message("  >>> ${MODULE_PACKAGE_DEPS}")

                    if (BUILD_SHARED)
                        foreach (file ${${module}_MOC})
                            get_filename_component(filename ${file} NAME)
                            qt4_wrap_CPP(${module}_MOC_SRC "${${module}_PATH}/${filename}")
                        endforeach()
                        add_library("${module}" SHARED
                            ${${module}_sources}
                            ${${module}_MOC_SRC})
                        target_link_libraries("${module}"
                            ${MODULE_PACKAGE_DEPS} ${${module}_LIBS})
                        set_target_properties("${module}" PROPERTIES
                            CLEAN_DIRECT_OUTPUT 1
                            LIBRARY_OUTPUT_DIRECTORY "${${module}_PATH}"
                            SOVERSION ${LIBRARY_VERSION}
                            VERSION ${LIBRARY_VERSION})
                        module_object_files("${module}" ${module} "${${module}_PACKAGE}_shared_objs")
                        install(TARGETS ${module} DESTINATION ${LIBRARY_INSTALL_DIR})
                    endif(BUILD_SHARED)

                    if (BUILD_STATIC)
                        foreach (file ${${module}_MOC})
                            get_filename_component(filename ${file} NAME)
                            qt4_wrap_CPP(${module}_MOC_SRC "${${module}_PATH}/${filename}")
                        endforeach()
                        add_library("${module}_static" STATIC
                            ${${module}_sources}
                            ${${module}_MOC_SRC})
                        foreach (d ${MODULE_PACKAGE_DEPS})
                            target_link_libraries("${module}_static" ${d}_static)
                        endforeach()
                        target_link_libraries("${module}_static" ${${module}_LIBS})
                        set_target_properties("${module}_static" PROPERTIES
                            CLEAN_DIRECT_OUTPUT 1 PREFIX ""
                            LIBRARY_OUTPUT_DIRECTORY "${${module}_PATH}"
                            OUTPUT_NAME "${${module}_PATH}/lib${module}")
                        module_object_files("${module}_static" ${module} "${${module}_PACKAGE}_static_objs")
                        install(TARGETS ${module}_static DESTINATION ${LIBRARY_INSTALL_DIR})
                    endif(BUILD_STATIC)

                endif(${${module}_PACKAGE} MATCHES "UNDEF")
            endforeach(module ${DECLARED_MODULES})
        endif (NOT BUILD_MODULES)

    endif(BUILD_PACKAGES)

    # Reset cache variables.
    set(DECLARED_PACKAGES "" CACHE INTERNAL "" FORCE)
    foreach (package ${DECLARED_PACKAGES})
        set(${package}_MODULES "" CACHE INTERNAL "" FORCE)
    endforeach()
    set(DECLARED_MODULES "" CACHE INTERNAL "" FORCE)

endmacro(CREATE_LIBRARY_TARGETS)


#-------------------------------------------------------------------------------
# Macro: CREATE_HEADER_INSTALL_TARGET
#-------------------------------------------------------------------------------
#
# Description
#   Adds an install target for copying headers from the project source code
#   directories to the install location '${CMAKE_INSTALL_PREFIX}/include'.
#   All header files with the extensions *.h and *.hpp are copied.
#
# Details:
#   This functions makes special care to allow for the case where the
#   install location is inside the source tree. This is the case, for example,
#   when installing to a local build directory inside the source tree.
#   Header folders matching build, release, debug, .settings, cmake, src,
#   .svn are excluded from the header tree installed.
#
# Usage
#   CREATE_HEADER_INSTALL_TARGET(HEADER_INSTALL_DIRECTORY)
#
# Arguments:
#   HEADER_INSTALL_DIRECTORY ... Directory in which to install headers extracted
#                                from the source tree.
#
macro(CREATE_HEADER_INSTALL_TARGET HEADER_INSTALL_DIRECTORY)
    # Install target for headers header tree.
    if (NOT ${CMAKE_INSTALL_PREFIX} MATCHES "/usr/local")
        string(REGEX MATCH "[^/|\\][a-zA-z0-9|_|-]+$" bin_dir ${CMAKE_BINARY_DIR})
        string(REGEX MATCH "[^/|\\]([a-zA-z0-9|_|-]+$)" install_dir ${CMAKE_INSTALL_PREFIX})
        string(COMPARE EQUAL ${CMAKE_INSTALL_PREFIX} ${CMAKE_BINARY_DIR} install_in_build_dir)
        if ($install_in_build_dir)
            set(install_include_dir "${bin_dir}/include")
        else ()
            set(install_include_dir "${bin_dir}/${install_dir}/include")
        endif()
    else()
        set(install_include_dir "${HEADER_INSTALL_DIRECTORY}")
    endif ()
    install(DIRECTORY "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/."
        DESTINATION ${HEADER_INSTALL_DIRECTORY}
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN ${install_include_dir} EXCLUDE
        PATTERN build* EXCLUDE
        PATTERN build EXCLUDE
        PATTERN release EXCLUDE
        PATTERN debug EXCLUDE
        PATTERN examples EXCLUDE
        PATTERN .settings EXCLUDE
        PATTERN images EXCLUDE
        PATTERN doc EXCLUDE
        PATTERN cmake EXCLUDE
        PATTERN src EXCLUDE
        PATTERN .svn EXCLUDE)
endmacro(CREATE_HEADER_INSTALL_TARGET)

















#===============================================================================
# UTILITY MACROS
#===============================================================================

macro(PRINT_MODULE_SUMMARY)
    message("=================================================================")
    message("* DECLARED MODULES:")
    foreach (mod ${DECLARED_MODULES})
        message("  - ${mod}")
    endforeach()
    foreach (mod ${DECLARED_MODULES})
        message("")
        message("  o MODULE [${mod}]")
        message("    = PATH: ${${mod}_PATH}")
        message("    = SRC: ")
        foreach (src ${${mod}_SRC})
            message("      - ${src}")
        endforeach()
        message("    = MOC: ")
        foreach (moc ${${mod}_MOC})
            message("      - ${moc}")
        endforeach()
        message("    = DEPS:")
        foreach (dep ${${mod}_DEPS})
            message("    - ${dep}")
        endforeach()
        message("    = LIBS: ")
        foreach (lib ${${mod}_LIBS})
            message("      - ${lib}")
        endforeach()
        message("    = PACKAGE: ${${mod}_PACKAGE}")
        message("    = LIBRARY: ${${mod}_LIBRARY}")
    endforeach()
    message("=================================================================")
endmacro(PRINT_MODULE_SUMMARY)


macro(PRINT_BUILD_OPTIONS)
    message(STATUS "Build system options:")
    message(STATUS "  BUILD_SHARED:    ${BUILD_SHARED}")
    message(STATUS "  BUILD_STATIC:    ${BUILD_STATIC}")
    message(STATUS "  BUILD_PACKAGES:  ${BUILD_PACKAGES}")
    message(STATUS "  BUILD_MODULES:   ${BUILD_MODULES}")
endmacro(PRINT_BUILD_OPTIONS)

#
# http://www.cmake.org/Wiki/CMakeMacroParseArguments
#
macro(PARSE_ARGUMENTS prefix arg_names option_names)
    set(DEFAULT_ARGS)
    foreach(arg_name ${arg_names})
        set(${prefix}_${arg_name})
    endforeach(arg_name)
    foreach(option ${option_names})
        set(${prefix}_${option} FALSE)
    endforeach(option)
    set(current_arg_name DEFAULT_ARGS)
    set(current_arg_list)
    foreach(arg ${ARGN})
        set(larg_names ${arg_names})
        list(FIND larg_names "${arg}" is_arg_name)
        if (is_arg_name GREATER -1)
            set(${prefix}_${current_arg_name} ${current_arg_list})
            set(current_arg_name ${arg})
            set(current_arg_list)
        else (is_arg_name GREATER -1)
            set(loption_names ${option_names})
            list(FIND loption_names "${arg}" is_option)
            if (is_option GREATER -1)
                set(${prefix}_${arg} TRUE)
            else (is_option GREATER -1)
                set(current_arg_list ${current_arg_list} ${arg})
            endif (is_option GREATER -1)
        endif (is_arg_name GREATER -1)
    endforeach(arg)
    set(${prefix}_${current_arg_name} ${current_arg_list})
endmacro(PARSE_ARGUMENTS)

#
# http://www.cmake.org/Wiki/CMakeMacroListOperations#CAR_and_CDR
#
macro(FIRST_ARG var)
    set(${var} ${ARGV1})
endmacro(FIRST_ARG)

macro(SECOND_ARG var junk)
    set(${var} ${ARGN})
endmacro(SECOND_ARG)

macro(MODULE_OBJECT_FILES target module objs)
    if (CMAKE_CONFIGURATION_TYPES)
        set(STATIC_OBJ_DIR ${CMAKE_CURRENT_BINARY_DIR}${target}.dir/${CMAKE_CFG_INTDIR})
    else()
        set(STATIC_OBJ_DIR ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${target}.dir)
    endif()
    get_property(target_sources TARGET ${target} PROPERTY SOURCES)
    foreach(sourcefile ${target_sources})
        if(IS_ABSOLUTE ${sourcefile})
            get_filename_component(source_name_temp "${sourcefile}" NAME)
            set(source_name "${${module}_PATH}/${source_name_temp}")
        else(IS_ABSOLUTE ${sourcefile})
            set(source_name "${sourcefile}")
        endif(IS_ABSOLUTE ${sourcefile})
        list(APPEND ${objs} ${source_name}${CMAKE_C_OUTPUT_EXTENSION})
    endforeach(sourcefile)
    ADD_DIR_PREFIX("${STATIC_OBJ_DIR}/" ${objs})
endmacro(MODULE_OBJECT_FILES)

macro(ADD_DIR_PREFIX prefix rootlist)
    set(outlist)
    foreach(root ${${rootlist}})
        if(IS_ABSOLUTE ${root})
            list(APPEND outlist ${root})
        else(IS_ABSOLUTE ${root})
            list(APPEND outlist ${prefix}${root})
        endif(IS_ABSOLUTE ${root})
    endforeach(root)
    set(${rootlist} ${outlist})
endmacro(ADD_DIR_PREFIX)

#
# Set build options defaults, if not set on the command line when running
# CMake.
#
macro(SET_BUILD_OPTION_DEFAULTS)
    if (NOT DEFINED BUILD_SHARED)
        set(BUILD_SHARED ON)
    endif()
    if (NOT DEFINED BUILD_STATIC)
        set(BUILD_STATIC OFF)
    endif()
    if (NOT DEFINED BUILD_PACKAGES)
        set(BUILD_PACKAGES ON)
    endif()
    if (NOT DEFINED BUILD_MODULES)
        set(BUILD_MODULES OFF)
    endif()
endmacro(SET_BUILD_OPTION_DEFAULTS)


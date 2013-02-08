# Declare a project library to be build from the specified subpackages.
macro(DECLARE_PROJECT_LIBRARY name)
    set(${name}_subpackage_DEPS ${ARGN})
    list(APPEND project_libraries ${name})
    file(WRITE "${SUBPACKAGE_WORK_DIR}/_${name}.cmake"
        "# Project library '${name}' - this file is auto-generated "
        "do not edit!\n\n"
    )
endmacro(DECLARE_PROJECT_LIBRARY)



# Create the project library specfied by the name argument.
macro(CREATE_PROJECT_LIBRARY name)

    # Die horribly if the project library has not been declared.
    if(NOT ${name}_subpackage_DEPS)
        message(FATAL_ERROR "Trying to build a project library '${name}' "
            "which has not been declared.")
    endif(NOT ${name}_subpackage_DEPS)

    # Only proceed if in single library mode.
    if(BUILD_SINGLE_LIB)

        # Create a target for a static project library.
        if(BUILD_SHARED)
            set(project_file "${SUBPACKAGE_WORK_DIR}/_${name}.cmake")
            set(shared_objects "")
            include(${project_file})
            foreach(obj ${shared_objects})
                set_source_files_properties(${obj} PROPERTIES GENERATED 1)
            endforeach(obj)
            add_library("${name}" SHARED ${shared_objects})
            set_target_properties("${name}" PROPERTIES CLEAN_DIRECT_OUTPUT 1)
            set_target_properties("${name}" PROPERTIES LINKER_LANGUAGE CXX)
            add_dependencies("${name}" ${shared_libs})
            install(TARGETS "${name}" DESTINATION ${LIBRARY_INSTALL_DIR})
            #list(REMOVE_DUPLICATES external_libs)
            target_link_libraries("${name}" ${external_libs})
        endif(BUILD_SHARED)

        # Create a target for a shared project library.
        if(BUILD_STATIC)
            set(project_file_static "${SUBPACKAGE_WORK_DIR}/_${name}_static.cmake")
            set(static_objects "")
            include(${project_file_static})
            foreach(obj ${static_objects})
                set_source_files_properties(${obj} PROPERTIES GENERATED 1)
            endforeach(obj)
            add_library("${name}_static" STATIC ${static_objects})
            set_target_properties("${name}_static" PROPERTIES CLEAN_DIRECT_OUTPUT 1)
            set_target_properties("${name}_static" PROPERTIES PREFIX "lib")
            set_target_properties("${name}_static" PROPERTIES OUTPUT_NAME "${name}")
            set_target_properties("${name}_static" PROPERTIES LINKER_LANGUAGE CXX)
            add_dependencies("${name}_static" ${static_libs})
            install(TARGETS "${name}_static" DESTINATION ${LIBRARY_INSTALL_DIR})
            #list(REMOVE_DUPLICATES external_libs)
            target_link_libraries("${name}_static" ${external_libs})
        endif(BUILD_STATIC)

    endif(BUILD_SINGLE_LIB)

endmacro(CREATE_PROJECT_LIBRARY)

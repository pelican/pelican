

macro(PROJECT_LIBRARY name)

    ## write project library file for linking modules in single library mode.

    if(BUILD_SINGLE_LIB)
        #message(STATUS "++ ${name}")
        
            # Set the name of the sub-package file.
        set(project_lib_file "${SUBPACKAGE_WORK_DIR}/_${name}.cmake")

        file(WRITE "${project_lib_file}"
            "set(${name}_dependencies ${ARGN})\n\n")
    
        ## link subpackage objects into single library
        #---------------------------------------------
        
        # Reset shared object list. 
        set(shared_objects "")
        
        # Load the shared objects for the single library target.
        foreach(subpackage ${ARGN})
            #message(STATUS "     ${subpackage}")
            include("${SUBPACKAGE_WORK_DIR}/${subpackage}.cmake")
            list(APPEND subpackage_external_libs ${subpackage_${subpackage}_external_LIBS})
        endforeach(subpackage)
        
        list(REMOVE_DUPLICATES subpackage_external_libs)
        
        file(APPEND "${project_lib_file}"
            "set(${name}_shared_objs ${shared_objects})\n\n")
        file(APPEND "${project_lib_file}"
            "set(${name}_libs ${subpackage_external_libs})\n\n")
        
        foreach(obj ${shared_objects}) 
            #message(STATUS "*** ${obj}")
            set_source_files_properties(${obj} PROPERTIES GENERATED 1)
        endforeach(obj)
        
        add_library("${name}" SHARED ${shared_objects})
        set_target_properties("${name}" PROPERTIES LINKER_LANGUAGE CXX)
        
        add_dependencies("${name}" ${shared_libs})
        install(TARGETS "${name}" DESTINATION ${LIBRARY_INSTALL_DIR})
        
        target_link_libraries("${name}" ${subpackage_external_libs})
        
    endif(BUILD_SINGLE_LIB)

endmacro(PROJECT_LIBRARY)




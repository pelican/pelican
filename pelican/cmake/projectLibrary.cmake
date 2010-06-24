
# Create the project library specfied by the name argument.
macro(CREATE_PROJECT_LIBRARY name)

    # Die horribly if the project library has not been declared.
    if(NOT ${name}_subpackage_DEPS)
        message(FATAL_ERROR "Trying to build a project library '${name}' " 
            "which has not been declared.")
    endif(NOT ${name}_subpackage_DEPS)

    ## write project library file for linking modules in single library mode.

    
    # Only proceed if in single library mode.
    if(BUILD_SINGLE_LIB)
    
        # Load the project library file to get objects / includes / link deps.    
    
            # Set the name of the sub-package file.
        #set(project_file "${SUBPACKAGE_WORK_DIR}/_${name}.cmake")

        # Reset shared object list. 
        #set(shared_objects "")
        
        # Load the project file.
        #include(${project_file})

        # Mark objects as generated ## why?               
        #foreach(obj ${shared_objects}) 
#            set_source_files_properties(${obj} PROPERTIES GENERATED 1)
        #endforeach(obj)
        
        # Create a target for the project library.
        #add_library("${name}" SHARED ${shared_objects})
        #set_target_properties("${name}" PROPERTIES LINKER_LANGUAGE CXX)
        #add_dependencies("${name}" ${shared_libs})
        
        # Add an install target for the project library.        
        #install(TARGETS "${name}" DESTINATION ${LIBRARY_INSTALL_DIR})
        
        # Link the project library with external library dependencies.
        #target_link_libraries("${name}" ${subpackage_external_libs})
        
    endif(BUILD_SINGLE_LIB)

endmacro(CREATE_PROJECT_LIBRARY)



# Declare a project library to be build from the specified subpackages.
macro(DECLARE_PROJECT_LIBRARY name)
    set(${name}_subpackage_DEPS ${ARGN})
    list(APPEND project_libraries ${name})
    
    file(WRITE "${SUBPACKAGE_WORK_DIR}/_${name}.cmake"
        "# Project library '${name}' - this file is auto-generated "
        "do not edit!\n\n"
    )
endmacro(DECLARE_PROJECT_LIBRARY)


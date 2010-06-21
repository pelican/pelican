

macro(PROJECT_LIBRARY name)

    ## write project library file for linking modules in single library mode.


    
    if(BUILD_SINGLE_LIB)
    
        ## link subpackage objects into single library
        
        foreach(subpackage ${ARGN})
            message(STATUS "+++++++++++++++ ${subpackage}")
        endforeach(subpackage)
        
                
        
    
    endif(BUILD_SINGLE_LIB)

endmacro(PROJECT_LIBRARY)




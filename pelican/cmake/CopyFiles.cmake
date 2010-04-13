macro(copy_files GLOBPAT DESTINATION target)

    file(GLOB COPY_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${GLOBPAT})

    # Adds a target to make all to copy files
#    add_custom_target(copy ALL
#        COMMENT "Copying files: ${GLOBPAT}")
    add_custom_target(${target} ALL
        COMMENT "Copying files: ${GLOBPAT}")



    foreach(FILENAME ${COPY_FILES})
        set(src "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}")
        set(dst "${DESTINATION}/${FILENAME}")
#        add_custom_command(TARGET copy COMMAND ${CMAKE_COMMAND} -E copy ${src} ${dst})
        add_custom_command(TARGET ${target} COMMAND ${CMAKE_COMMAND} -E copy ${src} ${dst})
    endforeach(FILENAME)

endmacro(copy_files)

macro(copy_files GLOBPAT DESTINATION)
file(GLOB COPY_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ${GLOBPAT})
  add_custom_target(copy ALL COMMENT "Copying files: ${GLOBPAT}")
  foreach(FILENAME ${COPY_FILES})
    set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}")
    set(DST "${DESTINATION}/${FILENAME}")

    add_custom_command(
      TARGET copy
      COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST}
      )
  endforeach(FILENAME)
endmacro(copy_files)

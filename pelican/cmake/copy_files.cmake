#
# Utility macros for copying files
#
# Last update: 9th January 2013
#

#
# Create a target to attach file copy commands to (if not already created)
#
set(target copy_files)
if (NOT TARGET ${target})
    add_custom_target(${target} ALL)
endif(NOT TARGET ${target})


#-------------------------------------------------------------------------------
# Macro: COPY_FILES
#-------------------------------------------------------------------------------
#
# Description:
#   Adds a command to the build target 'copy_files' which copies files matching
#   the specifeid globbing expression from the specified source directory to
#   the specified destination directory.  
#
# Usage:
#   COPY_FILES(SRC_DIR GLOB_PAT DST_DIR)
#
# Arguments:
#   - SRC_DIR   : The source directory containging files to be copied.
#   - GLOB_PAT  : globbing expression used to match files for copying.
#   - DST_DIR   : The destination directory where files are to be copied to.
#
# Example:
#   copy_files(${CMAKE_CURRENT_SOURCE_DIR} *.dat ${CMAKE_CURRENT_BINARY_DIR})
#       Will copy all files in the current source directory with the extension 
#       '.dat' into the current binary directory.
#
macro(COPY_FILES SRC_DIR GLOB_PAT DST_DIR)
    file(GLOB file_list 
        RELATIVE ${SRC_DIR} 
        ${SRC_DIR}/${GLOB_PAT})
    foreach(filename ${file_list})
        set(src "${SRC_DIR}/${filename}")
        set(dst "${DST_DIR}/${filename}")
        add_custom_command(TARGET ${target} 
            COMMAND ${CMAKE_COMMAND} -E copy ${src} ${dst})
    endforeach(filename)
endmacro(COPY_FILES)

#-------------------------------------------------------------------------------
# Macro: COPY_FILE
#-------------------------------------------------------------------------------
#
# Description:
#   Adds a command to the build target 'copy_files' which copies the specified
#   file to the specified destination. 
#
# Usage:
#   COPY_FILE(SRC DST)
#
# Arguments:
#   - SRC      : The source filename path (the file to be copied).
#   - DST      : The destiation filename path.
#
# Example:
#   copy_file(
#        ${CMAKE_CURRENT_SOURCE_DIR}/myfile.txt 
#        ${CMAKE_CURRENT_BINARY_DIR}/myfile.txt
#   )
#
macro(COPY_FILE SRC DST)
    add_custom_command(TARGET ${target}
        COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST})
endmacro(COPY_FILE)

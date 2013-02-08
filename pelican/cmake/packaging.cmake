#
# CMAKE packaging script
#
# Description:
# Adds tagets: 
#   1. dist (build the source package with included version file)
#   2. package (binary package)
#   3. package_source (source package) 
# 
# Notes:
#
# http://www.cmake.org/Wiki/CMake:CPackConfiguration
#
# http://www.cmake.org/Wiki/CMakeUserUseDebian
#
# http://wiki.clug.org.za/wiki/How_do_I_install_a_.deb_file_I_downloaded_without_compromising_dependencies%3F
# dpkg-scanpackages . /dev/null | gzip -c -9 > Packages.gz
# deb file:///home/debs /
#

set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/readme.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "University of Oxford, Oxford e-Research Centre.")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/copying.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY ON)
set(CPACK_SOURCE_IGNORE_FILES ${PROJECT_BINARY_DIR}/*)

if (WIN32)
    # http://www.itk.org/Wiki/CMake:Component_Install_With_CPack
    set(CPACK_GENERATOR "NSIS")
elseif (UNIX)
    #set(CPACK_GENERATOR "DEB;TGZ")
    set(CPACK_GENERATOR "TGZ")
    #if (APPLE )
    #    set(CPACK_GENERATOR "Bundle")
    #    set(CPACK_BUNDLE_NAME "pelican")
    #    set(CPACK_BUNDLE_PLIST
    #endif()
    set(CPACK_SOURCE_GENERATOR "ZIP")

    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "The PELICAN developer team.")
    set(CPACK_DEBIAN_PACKAGE_VERSION "${PROJECT_VERSION}")
    set(CPACK_DEBIAN_PACKAGE_SECTION "Science")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    # FIX THIS LINE
    #set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost (>=3.0), libqt4-dev (>=4.5) ")
endif ()
include(CPack)

add_custom_target(dist 
    COMMAND ${CMAKE_MAKE_PROGRAM} package_source
    COMMENT "Packaging Source files"
    VERBATIM)

#
# dependencies.cmake: Sets depencies for PELICAN.
#
# Required dependencies:
#   Qt4
#   Boost (program options)
#   CppUnit
#
# Sets the following variables:
#   PROJECT_LIBARRIES: Needed for linking with the libpelican.a
#

# === Find Dependencies.
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Qt4 COMPONENTS QtCore QtNetwork QtXml REQUIRED)
find_package(CppUnit REQUIRED)

# Global include directories set for all sub-packages
include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR} ## remove when all includes fixed for QtCore/... etc
)

# === List of all libraries for single library install.
set(PROJECT_LIBRARIES
    pelican
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

#
# dependencies.cmake: Sets depencies for PELICAN.
#
# Required dependencies:
#   Qt4
#   CppUnit
#   CFitsio
#   Boost_program_options
#   CBLAS (either provided by ATLAS or MKL)
#   LAPACK (either provided by ATLAS or MKL)
#
# Optional dependencies:
#
#

# === Find Dependencies.
find_package(Boost COMPONENTS program_options REQUIRED)
set(dependencies Qt4 CppUnit CFitsio)
PACKAGE_DEPENDENCIES(${dependencies}) # This calls find_package() ...

# === Find cblas and lapack from MKL if availiable, otherwise elsewhere.
set(pelican_mkl false)
set(IGNORE_MKL true) ## HACK TO TURN OFF LOOKING FOR MKL AS FINDMKL IS BROKEN.... FIXME!
if (NOT IGNORE_MKL)
    find_package(MKL) # Broken somewhere... :\
endif (NOT IGNORE_MKL)
if (MKL_FOUND)
    add_definitions(-DUSING_MKL)
    set(pelican_math_libs ${MKL_LIBRARIES})
    set(pelican_mkl true)
else (MKL_FOUND)
    find_package(CBLAS REQUIRED)
    find_package(LAPACK REQUIRED)
    set(pelican_math_libs ${LAPACK_LIBRARIES} ${CBLAS_LIBRARIES})
endif (MKL_FOUND)

# === List of all libraries for single library install.
set(PROJECT_LIBRARIES pelican
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${pelican_math_libs}
    ${CFITSIO_LIBRARIES}
)


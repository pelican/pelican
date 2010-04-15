#
# Saves build configuration information to file for use with the
# FindPelicanInstall module
#
file(WRITE cmake/pelicanBuildConfig.cmake "set(pelican_mkl ${pelican_mkl})\n")
#file(APPEND cmake/pelicanBuildConfig.cmake "set(pelican_intel_openmp ${pelican_intel_openmp})")

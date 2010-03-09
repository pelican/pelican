Welcome To Pelican
==================
The Pipeline for Extensible, Lightweight Imaging and CAlibratioN


Code Build Dependencies
=======================
Required:   Qt4
            boost_program_options
            blas (atlas, gsl or mkl)
            lapack
            cfitsio
Optional:   openmp (compiler)


Documentation Build Dependencies:
================================================================================
Required: Doxygen
Optional: LATEX (texlive ... on ubuntu)
          Graphviz (?)
          pdflatex

Using the Intel C/C++ compiler.
================================================================================
Before building the makefiles with cmake:
    export CC=icc
    export CXX=icpc


CMAKE build options
================================================================================
Build:
    -DCMAKE_BUILD_TYPE=release [default = debug]:
    -DOPENMP=<bool> [default = false]: Use threaded pipeline modules computation.
    -DIGNORE_MKL=<bool> [default = false]: Don't use Intel MKL for cblas and lapack.
    -DMKL_THREADED=<bool> [default = false]: Use threaded MKL.

Debugging:
    -DTIMER=<bool> [default = false]: Enables printing of various timer messages.



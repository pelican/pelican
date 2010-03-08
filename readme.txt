Welcome To Pelican
==================
The Pipeline for Extensible, Lightweight Imaging and CAlibratioN

Code Build Dependencies
=======================
required:   Qt4
            boost_program_options
            blas (atlas, gsl or mkl)
            lapack
            cfitsio

optional:   openmp (compiler)

Documentation Build Dependencies
=================================
required: Doxygen
optional: LATEX (texlive ... on ubuntu)
          Graphviz (?)
          pdflatex

setting the intel compiler
=====================
export CC=icc
export CXX=icpc
cmake ....


cmake build options
===============================
build in release mode: -DCMAKE_BUILD_TYPE=release [default = debug]
turn on the timer: -DTIMER=true [default = false]
use openmp: -DOPENMP=true [default = false]
dont use mkl: -DNO_MKL=true [default = false]
dont use any blas: -DNO_CBLAS=true [default = false]
dont use threaded mkl: -DMKL_THREADED=true [default = false]


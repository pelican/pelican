Welcome To Pelican
==================
The Pipeline for Extensible, Lightweight Imaging and CAlibratioN


Code Build Dependencies
=======================
Required:   Qt4
            boost_program_option (http://www.boost.org)
            blas (atlas, gsl or mkl)
            lapack
            cfitsio (http://heasarc.gsfc.nasa.gov/fitsio/)
            cppunit

Optional:   openmp (compiler)


Documentation Build Dependencies:
================================================================================
Required: Doxygen
Optional: LATEX (texlive ... on ubuntu)
          Graphviz (?)
          pdflatex

CMAKE build options
================================================================================
Build:
    -DCMAKE_BUILD_TYPE=release [default = debug]:
        Build in release / debug mode.

    -DOPENMP=<bool> [default = false]:
        Use threaded pipeline modules computation.

    -DIGNORE_MKL=<bool> [default = false]:
        Don't use Intel MKL for cblas and lapack.

    -DMKL_THREADED=<bool> [default = false]:
        Use threaded MKL library (not recomended).

    -DINTEL=true [default=false]:
        Use the intel compiler. (icc and icpc must be in the system path)

Debugging:
    -DTIMER=<bool> [default = false]:
        Enables printing of various timer messages.

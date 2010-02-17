#ifndef VISGEN_H
#define VISGEN_H

#include <vector>
#include <complex>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

/**
 * @file VisGen.h
 */

namespace pelican {

/**
 * @class VisGen
 *  
 * @brief
 * 
 * @details
 * 
 */

class VisGen
{
    public:
        VisGen(int argc, char** argv);
        ~VisGen();
        void generate();
        void write(const std::string& fileName);

    private:        
        void _getCommandLineArgs(int argc, char** argv);
        
    private:
        typedef std::complex<float> complex_t;
        bool _binary;
        int _nAnt;
        int _nChan;
        int _nPol;
        std::vector<complex_t> _data;
};

} // namespace pelican
#endif // VISGEN_H 

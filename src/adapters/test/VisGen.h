#ifndef VISGEN_H
#define VISGEN_H

#include <QDataStream>
#include <vector>
#include <complex>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <data/DataBlob.h>

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
 */

class VisGen
{
    public:

        VisGen() {_binary = true;}
        VisGen(int argc, char** argv);
        ~VisGen();

        void generate() {
            _generate(_nAnt, _nChan, _nPol);
        }

        void generate(int nAnt, int nChan, int nPol) {
            _binary = true;
            _generate(nAnt, nChan, nPol);
        }

        void write(const std::string& fileName);

        void print();

        QByteArray dataStream();

        complex_t* data() { return &_data[0]; }

        int& nAntennas() { return _nAnt; }

        int& nChannels() { return _nChan; }

        int& nPolarisations() { return _nPol; }

        unsigned size() { return _data.size() * sizeof(complex_t); }

    private:
        void _generate(int nAnt, int nChan, int nPol);
        void _getCommandLineArgs(int argc, char** argv);

    private:
        bool _binary;
        int _nAnt;
        int _nChan;
        int _nPol;
        std::vector<complex_t> _data;
};

} // namespace pelican
#endif // VISGEN_H

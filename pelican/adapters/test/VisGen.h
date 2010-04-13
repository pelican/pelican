#ifndef VISGEN_H
#define VISGEN_H

#include "pelican/data/DataBlob.h"

#include <QByteArray>
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
 * Generates a known set of visibility data with the correct dimensions.
 *
 * @details
 * This class generates a visibility data set with the same memory
 * layout as used by the single-station LOFAR data files.
 *
 * It is used for testing purposes only.
 */
class VisGen
{
    public:
        /// Default constructor.
        VisGen() {_binary = true;}

        /// Constructor to parse command-line arguments.
        VisGen(int argc, char** argv);

        /// Returns a QByteArray containing the visibility data.
        QByteArray byteArray();

        /// Returns a pointer to the start of the data block.
        complex_t* data() { return (_data.size() > 0) ? &_data[0] : NULL; }

        /// Generates a visibility data set using the pre-set dimensions.
        void generate() {
            _generate(_nAnt, _nChan, _nPol);
        }

        /// Generates a visibility data set using the given dimensions.
        void generate(int nAnt, int nChan, int nPol) {
            _binary = true;
            _generate(nAnt, nChan, nPol);
        }

        /// Returns the number of antennas in the visibility data.
        int& nAntennas() { return _nAnt; }

        /// Returns the number of channels in the visibility data.
        int& nChannels() { return _nChan; }

        /// Returns the number of polarisations in the visibility data.
        int& nPolarisations() { return _nPol; }

        /// Prints the generated visibility data to standard output.
        void print();

        /// Returns the size of the generated data block in bytes.
        unsigned size() { return _data.size() * sizeof(complex_t); }

        /// Writes the generated visibility data to file.
        void write(const std::string& fileName);

    private:
        /// Called by generate() to generate the visibility data.
        void _generate(int nAnt, int nChan, int nPol);

        /// Parses the command line arguments, if used in a stand-alone binary.
        void _getCommandLineArgs(int argc, char** argv);

    private:
        bool _binary; ///< Flag set if using binary output format.
        int _nAnt;    ///< Number of antennas in the generated data.
        int _nChan;   ///< Number of channels in the generated data.
        int _nPol;    ///< Number of polarisations in the generated data.
        std::vector<complex_t> _data; ///< The generated visibility data.
};

} // namespace pelican

#endif // VISGEN_H

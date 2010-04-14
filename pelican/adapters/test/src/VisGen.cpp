#include "pelican/adapters/test/VisGen.h"
#include <iomanip>
#include <QString>

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Constructor to parse and print out the command line arguments.
 */
VisGen::VisGen(int argc, char** argv)
{
    _binary = true;
    _getCommandLineArgs(argc, argv);
    std::string binary = (_binary) ? "yes" : "no";
    std::cout << "----------------------------------------\n";
    std::cout << "antennas      = " << _nAnt << std::endl;
    std::cout << "polarisations = " << _nPol << std::endl;
    std::cout << "channels      = " << _nChan << std::endl;
    std::cout << "binary        = " << binary << std::endl;
    std::cout << "----------------------------------------\n";
}

/**
 * @details
 * Returns a QByteArray containing the generated visibility data.
 */
QByteArray VisGen::byteArray()
{
    if (_data.empty())
        throw QString("VisGen: Data not yet generated.");

    char* data = reinterpret_cast<char*>(&_data[0]);
    return QByteArray(data, sizeof(complex_t) * _data.size());
}

/**
 * @details
 * Print the generated data.
 */
void VisGen::print()
{
    int nPointsPerChan = _nAnt * _nPol * _nAnt * _nPol;
    std::cout << std::fixed << std::setprecision(2);

    for (int c = 0; c < _nChan; c++) {
        int cIndex = c * nPointsPerChan;

        for (int j = 0; j < _nAnt; j++) {
            for (int pj = 0; pj < _nPol; pj++) {
                int jIndex = (j * _nPol + pj) * (_nAnt * _nPol);

                for (int i = 0; i < _nAnt; i++) {
                    for (int pi = 0; pi < _nPol; pi++) {

                        int index = cIndex + jIndex + (i * _nPol + pi);
                        std::cout << _data[index];
                    }
                }
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }
}

/**
 * @details
 * Writes the generated data as either binary or ASCII to the given
 * \p fileName.
 *
 * @param[in] fileName File name of the data to write.
 */
void VisGen::write(const std::string& fileName)
{
    /* Open the output data stream */
    std::ofstream file(fileName.c_str(), std::ios::out | std::ios::binary);

    /* Write data to file either as binary or ASCII */
    if (_binary) {
        file.write(reinterpret_cast<char*>(&_data[0]),
                sizeof(complex_t) * _data.size());
    }
    else {
        int nPointsPerChan = _nAnt * _nPol * _nAnt * _nPol;
        file << std::fixed << std::setprecision(2);

        for (int c = 0; c < _nChan; c++) {
            int cIndex = c * nPointsPerChan;

            for (int j = 0; j < _nAnt; j++) {
                for (int pj = 0; pj < _nPol; pj++) {
                    int jIndex = (j * _nPol + pj) * (_nAnt * _nPol);

                    for (int i = 0; i < _nAnt; i++) {
                        for (int pi = 0; pi < _nPol; pi++) {

                            int index = cIndex + jIndex + (i * _nPol + pi);
                            file << _data[index];
                        }
                    }
                    file << std::endl;
                }
            }
            file << std::endl;
        }
    }

    /* Close the file */
    file.close();
}

/**
 * @details
 * Parses command line arguments and stores the parameters.
 */
void VisGen::_getCommandLineArgs(int argc, char** argv)
{
    if (argc != 5) {
        std::cout << "Usage:\n";
        std::cout << "    $ " << argv[0] << " nAnt nPol nChan 0=(binary)\n";
        exit(66);
    }
    else {
        _nAnt = atoi(argv[1]);
        _nPol = atoi(argv[2]);
        _nChan = atoi(argv[3]);
        int b = atoi(argv[4]);
        _binary = (b == 0) ? true : false;
    }
    if (_nAnt < 1 || _nPol < 1 || _nPol > 2 || _nChan < 1) {
        std::cout << "Error in data dimensions\n";
        exit(666);
    }
}

/**
 * @details
 * Generates the visibility data with the given dimensions.
 *
 * @param[in] nAnt Number of antennas in the generated data.
 * @param[in] nChan Number of channels in the generated data.
 * @param[in] nPol Number of polarisations in the generated data.
 */
void VisGen::_generate(int nAnt, int nChan, int nPol)
{
    _nAnt = nAnt;
    _nChan = nChan;
    _nPol = nPol;
    int nPoints = nAnt * nPol * nAnt * nPol * nChan;
    int nPointsPerChan = nAnt * nPol * nAnt * nPol;
    _data.resize(nPoints);

    for (int c = 0; c < nChan; c++) {
        int cIndex = c * nPointsPerChan;

        for (int j = 0; j < nAnt; j++) {
            for (int pj = 0; pj < nPol; pj++) {
                int jIndex = (j * nPol + pj) * (nAnt * nPol);

                for (int i = 0; i < nAnt; i++) {
                    for (int pi = 0; pi < nPol; pi++) {

                        int index = cIndex + jIndex + (i * nPol + pi);

                        // CRITICAL: Keep this for the test to work.
                        // =====================================================
                        float aj = float(j) / float(100.0);
                        float ai = float(i) / float(100.0);
                        float re = float(c) + aj + ai;
                        float im = float(c) + aj + ai;
                        re += (pi == pj) ? float(1000.0) * pi : float(-99999.9);
                        im += (pi == pj) ? float(1000.0) * pi : float(-99999.9);
                        // =====================================================

                        _data[index] = complex_t(re, im);
                    }
                } // loop over columns

            }
        } // loop over rows

    } // loop over channels
}

} // namespace pelican

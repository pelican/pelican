#include "VisGen.h"
#include "utility/memCheck.h"
#include <iomanip>
#include <QString>
#include <QByteArray>

namespace pelican {


VisGen::VisGen(int argc, char** argv)
{
    _binary = true;
    _getCommandLineArgs(argc, argv);
    std::cout << "----------------------------------------\n";
    std::cout << "nAnt   = " << _nAnt << std::endl;
    std::cout << "nPol   = " << _nPol << std::endl;
    std::cout << "nChan  = " << _nChan << std::endl;
    std::string binary = (_binary) ? "yes" : "no";
    std::cout << "binary = " << binary << std::endl;
    std::cout << "----------------------------------------\n";
}

VisGen::~VisGen()
{
}

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


void VisGen::_generate(int nAnt, int nChan, int nPol)
{
    _nAnt = nAnt;
    _nChan = nChan;
    _nPol = nPol;
    int nPoints = nAnt * nPol * nAnt * nPol * nChan;
    int nPointsPerChan = nAnt * nPol * nAnt * nPol;
    std::cout << "Number of data points = " << nPoints << "\n";
    _data.resize(nPoints);

    for (int c = 0; c < nChan; c++) {
        int cIndex = c * nPointsPerChan;

        for (int j = 0; j < nAnt; j++) {
            for (int pj = 0; pj < nPol; pj++) {
                int jIndex = (j * nPol + pj) * (nAnt * nPol);

                for (int i = 0; i < nAnt; i++) {
                    for (int pi = 0; pi < nPol; pi++) {

                        int index = cIndex + jIndex + (i * nPol + pi);
                        float aj = static_cast<float>(j) / 100.0;
                        float ai = static_cast<float>(i) / 100.0;
                        float re = static_cast<float>(c) + aj + ai;
                        float im = static_cast<float>(c) + aj + ai;
//                        re += (pi == pj) ? static_cast<float>(c) + 1.0 : 0.0;
//                        im += (pi == pj) ? static_cast<float>(c) + 1.0 : 0.0;
                        re += (pi == pj) ? 0.0 : -99999.9999;
                        im += (pi == pj) ? 0.0 : -99999.9999;
                        _data[index] = complex_t(re, im);
                    }
                } // loop over columns

            }
        } // loop over rows

    } // loop over channels
}



void VisGen::print()
{
    std::cout << "Data size = " << _data.size() * sizeof(complex_t) << " bytes.\n";
    int nPointsPerChan = _nAnt * _nPol * _nAnt * _nPol;
    std::cout << std::setprecision(2);
    std::cout << std::fixed;

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



void VisGen::write(const std::string& fileName)
{
    std::cout << "Data size = " << _data.size() * sizeof(complex_t) << " bytes.\n";
    std::ofstream file;
    file.open(fileName.c_str(), std::ios::out | std::ios::binary);
    if (!file.good()) std::cout << "eek\n";

    if (_binary) {
        file.write(reinterpret_cast<char*>(&_data[0]),
                sizeof(complex_t) * _data.size());
    }
    else {
        int nPointsPerChan = _nAnt * _nPol * _nAnt * _nPol;
        file << std::setprecision(2);
        file << std::fixed;

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
    file.close();
//    _data.clear();
}


QByteArray VisGen::dataStream()
{
    if (_data.empty())
        throw QString("VisGen: data not yet genrated.");

    char* data = reinterpret_cast<char*>(&_data[0]);
    return QByteArray(data, sizeof(complex_t) * _data.size());
}

} // namespace pelican

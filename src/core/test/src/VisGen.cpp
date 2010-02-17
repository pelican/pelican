#include "VisGen.h"
#include "utility/memCheck.h"

namespace pelican {


VisGen::VisGen(int argc, char** argv)
{
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


void VisGen::generate()
{
    int nPoints = _nAnt * _nPol * _nAnt * _nPol * _nChan;
    int nPointsPerChan = _nAnt * _nPol * _nAnt * _nPol;
    std::cout << nPoints << "\n";
    _data.resize(nPoints);

    for (int c = 0; c < _nChan; c++) {
        int cIndex = c * nPointsPerChan;

        for (int j = 0; j < _nAnt; j++) {
            for (int pj = 0; pj < _nPol; pj++) {
                int jIndex = (j * _nPol + pj) * (_nAnt * _nPol);

                for (int i = 0; i < _nAnt; i++) {
                    for (int pi = 0; pi < _nPol; pi++) {

                        int index = cIndex + jIndex + (i * _nPol + pi);
                        float re = static_cast<float>(c);
                        float im = static_cast<float>(c);
                            re += (pi == pj) ? 1.0 : 0.0;
                            im += (pi == pj) ? 1.0 : 0.0;
                            _data[index] = complex_t(re, im);
                        }
                    } // loop over columns

                }
            } // loop over rows

        } // loop over channels
    }


void VisGen::write(const std::string& fileName)
{
    std::cout << "----------------------------------------\n";
    std::cout << "Data size: " << _data.size() * sizeof(complex_t) << ".\n";
    std::ofstream file;
    file.open(fileName.c_str(), std::ios::out | std::ios::binary);

    if (_binary) {
        file.write(reinterpret_cast<char*>(&_data[0]),
                sizeof(complex_t) * _data.size());
    }
    else {
        int nPointsPerChan = _nAnt * _nPol * _nAnt * _nPol;

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
    _data.clear();
}
} // namespace pelican

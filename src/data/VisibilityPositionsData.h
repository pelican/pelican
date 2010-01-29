#ifndef VISIBILITYPOSITIONSDATA_H
#define VISIBILITYPOSITIONSDATA_H

#include "data/DataBlob.h"
#include <vector>

/**
 * @file AntennaPositionsData.h
 */

namespace pelican {

/**
 * @class VisibilityPositions
 *  
 * @brief
 * Visibility positions container class.
 * 
 * @details
 * This class holds visibility positions for a telescope.
 * Positions are stored for a reference frequency channel and scaled upon demand.
 * This approach was chosen to save nChannels storage.
 *
 * Visibility positions are indexed by channel then either a pair of antenna ids
 * or and either a pair of antenna index from a list of visibility id's.
 */


class VisibilityPositions : public DataBlob
{
    public:
        /// Constructor
        VisibilityPositions();

        /// Constructor assigning memory for the visibility positions
        VisibilityPositions(const unsigned int nAntennas);

        /// Image data destructor.
        ~VisibilityPositions();

    public:
        /// Assign memory for visibility positions
        void assign(const unsigned int nAntennas);

        /// Clears the visibility positions data
        void clear();

        /// Returns the frequency scaling factor for the specified channel
        const double freqScaleFactor(const unsigned int channel) {
            return (_refFreq + (channel - _refChannel) * _freqInc) / _refFreq;
        }

    public: // accessor methods

        /// Returns the number of antenna for which visibility positions are stored.
        int nAntennas() const { return _nAntennas; }

        /// Returns the number of visibility coordinates held.
        int nVis() const { return _u.size(); }

        /// Returns a reference to the reference frequency channel id.
        int& refChannel() { return _refChannel; }

        /// Returns a reference to the frequency of the reference channel
        double& refFreq() { return _refFreq; }

        /// Frequency a reference to the increment between channels
        double& freqInc() { return _freqInc; }

        /// Returns the u coordinate for the visibility i at the reference frequency
        real_t& u(const unsigned int i) { return _u[i]; }

        /// Returns the u coordinate for the visibility indexed by antennas at the reference frequency
        real_t& u(const unsigned int a1, const unsigned int a2) {
            return _u[a2 * _nAntennas + a1];
        }

        /// Returns the u coordinate for the visibility indexed by antennas at the specified frequency
        const real_t u(const unsigned int a1, const unsigned int a2, const unsigned int channel) {
            return _u[a2 * _nAntennas + a1] * freqScaleFactor(channel);
        }

        /// Returns the v coordinate for the visibility i at the reference frequency
        real_t& v(const unsigned int i) { return _v[i]; }

        /// Returns the v coordinate for the visibility indexed by antennas at the reference frequency
        real_t& v(const unsigned int a1, const unsigned int a2) {
            return _v[a2 * _nAntennas + a1];
        }

        /// Returns the v coordinate for the visibility indexed by antennas at the specified frequency
        const real_t v(const unsigned int a1, const unsigned int a2, const unsigned int channel) {
            return static_cast<real_t>(_v[a2 * _nAntennas + a1] * freqScaleFactor(channel));
        }

        /// Returns the w coordinate for the visibility i at the reference frequency
        real_t& w(const unsigned int i) { return _w[i]; }

        /// Returns the w coordinate for the visibility indexed by antennas at the reference frequency
        real_t& w(const unsigned int a1, const unsigned int a2) { return _w[a2 * _nAntennas + a1]; }

        /// Returns the w coordinate for the visibility indexed by antennas at the specified frequency
        const real_t w(const unsigned int a1, const unsigned int a2, const unsigned int channel) {
            return _w[a2 * _nAntennas + a1] * freqScaleFactor(channel);
        }

        /// Returns a pointer to the u antenna positions.
        real_t* uPtr() { return (_u.size() > 0) ? &_u[0] : NULL; }

        /// Returns a pointer to the v antenna positions.
        real_t* vPtr() { return (_v.size() > 0) ? &_v[0] : NULL; }

        /// Returns a pointer to the w antenna positions.
        real_t* wPtr() { return (_w.size() > 0) ? &_w[0] : NULL; }

    private:
        /// The number of antennas
        int _nAntennas;
        /// Reference frequency channel id.
        int _refChannel;
        /// Frequency of the reference channel
        double _refFreq;
        /// Frequency increment between channels
        double _freqInc;
        /// u coordinate of visibility
        std::vector<real_t> _u;
        /// v coordinate of visibility
        std::vector<real_t> _v;
        /// w coordinate of visibility
        std::vector<real_t> _w;
};


} // namespace pelican

#endif // VISIBILITYPOSITIONSDATA_H

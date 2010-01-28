#ifndef VISIBILITYPOSITIONSDATA_H
#define VISIBILITYPOSITIONSDATA_H

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
        double freqScale(const unsigned int channel) {
            return (_refFreq + (channel - _refChannel) * _freqInc) / _refFreq;
        }

    public: // accessor methods

        /// Returns the number of antenna for which visibility positions are stored.
        int nAntennas() const { return _nAntennas; }

        /// Returns the number of visibility coordinates held.
        int nVis() const { return _u.size(); }

        /// Returns the reference frequency channel id.
        int refChannel() const { return _refChannel; }

        /// Sets the reference channel id
        void setRefChannel(const unsigned int channel) { _refChannel = channel; }

        /// Returns the frequency of the reference channel
        double refFreq() const { return _refFreq; }

        /// Sets the frequency of the reference channel
        void setRefFreq(const double value) { _refFreq = value; }

        /// Frequency increment between channels
        double freqInc() const { return _freqInc; }

        /// Sets the frequency increment between channels
        double setFreqInc(const double value) { _freqInc = value; }

        /// Returns the u coordinate for the visibility i at the reference frequency
        real_t& u(const unsigned int i) const { return _u[i]; }

        /// Returns the u coordinate for the visibility indexed by antennas at the reference frequency
        real_t& u(const unsigned int a1, const unsigned int a2) const {
            _u[a2 * _nAntennas + a1];
        }

        /// Returns the u coordinate for the visibility indexed by antennas at the specified frequency
        real_t& u(const unsigned int a1, const unsigned int a2, const unsigned int channel) const {
            return static_cast<real_t>(_u[a2 * _nAntennas + a1] * freqScale(channel));
        }

        /// Sets the u coordinate of the visibility i to the value specified for the reference channel
        void setU(const unsigned int i, const real_t value) { _u[i] = value; }

        /// Sets the u coordinate of the visibility indexed by antennas to the value specified for the reference channel
        void setU(const unsigned int a1, const unsigned int a2, const real_t value) {
            _u[a2 * _nAntennas + a1] = value;
        }

        /// Returns the v coordinate for the visibility i at the reference frequency
        real_t& v(const unsigned int i) const { return _v[i]; }

        /// Returns the v coordinate for the visibility indexed by antennas at the reference frequency
        real_t& v(const unsigned int a1, const unsigned int a2) const {
            _u[a2 * _nAntennas + a1];
        }

        /// Returns the v coordinate for the visibility indexed by antennas at the specified frequency
        real_t& v(const unsigned int a1, const unsigned int a2, const unsigned int channel) const {
            return static_cast<real_t>(_v[a2 * _nAntennas + a1] * freqScale(channel));
        }

        /// Sets the v coordinate of the visibility i to the value specified for the reference channel
        void setV(const unsigned int i, const real_t value) { _v[i] = value; }

        /// Sets the v coordinate of the visibility indexed by antennas to the value specified for the reference channel
        void setV(const unsigned int a1, const unsigned int a2, const real_t value) {
            _v[a2 * _nAntennas + a1] = value;
        }

        /// Returns the w coordinate for the visibility i at the reference frequency
        real_t& w(const unsigned int i) const { return _w[i]; }

        /// Returns the w coordinate for the visibility indexed by antennas at the reference frequency
        real_t& w(const unsigned int a1, const unsigned int a2) const {
            _w[a2 * _nAntennas + a1];
        }

        /// Returns the w coordinate for the visibility indexed by antennas at the specified frequency
        real_t& w(const unsigned int a1, const unsigned int a2, const unsigned int channel) const {
            return static_cast<real_t>(_w[a2 * _nAntennas + a1] * freqScale(channel));
        }

        /// Sets the w coordinate of the visibility i to the value specified for the reference channel
        void setW(const unsigned int i, const real_t value) { _w[i] = value; }

        /// Sets the w coordinate of the visibility indexed by antennas to the value specified for the reference channel
        void setW(const unsigned int a1, const unsigned int a2, const real_t value) {
            _w[a2 * _nAntennas + a1] = value;
        }

        /// Returns a pointer to the u antenna positions.
        real_t* uPtr() { return _u.size > 0 ? &_u[0] : NULL; }

        /// Returns a pointer to the v antenna positions.
        real_t* vPtr() { return _v.size > 0 ? &_v[0] : NULL; }

        /// Returns a pointer to the w antenna positions.
        real_t* wPtr() { return _w.size > 0 ? &_w[0] : NULL; }

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

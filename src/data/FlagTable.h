#ifndef FLAGTABLE_H
#define FLAGTABLE_H

#include "data/AntennaMatrixData.h"
#include <vector>

using std::vector;

/**
 * @file FlagTable.h
 */

namespace pelican {

/**
 * @class FlagTable
 *  
 * @brief
 * Holds flag table data.
 * 
 * @details
 * This class holds flag table data. It inherits
 * the AntennaMatrixData<unsigned char> base class, which is used to define
 * the storage format.
 *
 * @section FlagTable_Flagging Flagging Data
 *
 * To flag the data from one antenna pair (\p ai, \p aj) at channel \p c
 * and polarisation \p p, use the flag() method.
 *
 * To flag all the data from a single antenna \p a at channel \p c and
 * polarisation \p p, use flagAntenna(). An overloaded version of flag() is
 * provided for the same operation.
 *
 * To flag the data from all antennas in a given channel \p c and
 * polarisation \p, use flagChannel(). An overloaded version of flag() is
 * provided for the same operation.
 *
 * @section FlagTable_Inspecting Inspecting Flags
 *
 * To return the flags for a given antenna pair (\p ai, \p aj) at channel \p c
 * and polarisation \p p, use the flags() method. An overloaded version returns
 * the flags for the autocorrelation (antenna \p a, only).
 *
 * To return the number of antennas flagged using flagAntenna() for
 * channel \p c and polarisation \p p, use nFlaggedAntennas(). Similarly, to
 * return the number of channels flagged using flagChannel() for
 * polarisation \p p, use nFlaggedChannels().
 *
 * A reference to the list of all the antennas flagged using flagAntenna() can
 * be obtained by calling flaggedAntennas(). A pointer to the first element of
 * the list can be obtained by calling flaggedAntennaPtr(). Similarly, a
 * reference to the list of all the channels flagged using flagChannel() can
 * be obtained by calling flaggedChannels(). A pointer to the first element of
 * the list can be obtained by calling flaggedChannelPtr().
 */
class FlagTable : public AntennaMatrixData<unsigned char>
{
    private:
        /// List of antennas flagged for each polarisation and channel.
        /// Dereference using [polarisation][channel][antenna].
        vector< vector< vector<unsigned> > > _flaggedAntennas;

        /// List of channels flagged for each polarisation.
        /// Dereference using [polarisation][channel].
        vector< vector<unsigned> > _flaggedChannels;

    public:
        /// Enumerator describing the flag reason code.
        enum {
            FLAG_STOKES_USE = 0x1,
            FLAG_RFI_BAD = 0x2,
            FLAG_UVDIST = 0x4,
            FLAG_AUTOCORR = 0x8
        };

        /// Constructs an empty data cube.
        /// The constructed data cube has zero size.
        FlagTable() : AntennaMatrixData<unsigned char>() {}

        /// Constructs a pre-sized, empty cube.
        /// The cube is pre-sized using the given parameters.
        ///
        /// @param[in] antennas The number of antennas in the visibility matrix.
        /// @param[in] channels The number of frequency channels.
        /// @param[in] polarisations The number of polarisations.
        FlagTable(const unsigned antennas, const unsigned channels,
                const unsigned polarisations) :
                    AntennaMatrixData<unsigned char>(antennas, channels, polarisations) {
            _flaggedChannels.resize(polarisations);
            _flaggedAntennas.resize(polarisations);
            for (unsigned p = 0; p < polarisations; p++) {
                _flaggedAntennas[p].resize(channels);
            }
        }

        /// Destructor.
        ~FlagTable() {}

        /// Clears the flag table.
        void clear() {
            _flaggedAntennas.clear();
            _flaggedChannels.clear();
        }

        /// Flags a single antenna pair.
        /// Adds an entry to the flag table for the given antenna pair at
        /// (\p ai, \p aj), channel \p c and polarisation \p p, with reason
        /// code \p reason.
        void flag(const unsigned ai, const unsigned aj, const unsigned c,
                const unsigned p, const unsigned char reason) {
            const unsigned index = ai + _nAntennas * (aj + _nAntennas * (c + _nChannels * p));
            _data[index] = reason;
        }

        /// Flags all the data from the given antenna.
        /// Flags all the data from the given antenna \p a at channel \p c and
        /// polarisation \p p, with reason code \p reason.
        void flag(const unsigned a, const unsigned c, const unsigned p,
                const unsigned char reason) {
            _flaggedAntennas[p][c].push_back(a);
            for (unsigned ai = 0; ai < _nAntennas; ai++) {
                flag(ai, a, c, p, reason);
                flag(a, ai, c, p, reason);
            }
        }

        /// Flags all antennas in the given channel.
        /// Flags all antennas in the given channel \p c at polarisation \p p,
        /// with reason code \p reason.
        void flag(const unsigned c, const unsigned p,
                const unsigned char reason) {
            _flaggedChannels[p].push_back(c);
            for (unsigned a = 0; a < _nAntennas; a++) flag(a, c, p, reason);
        }

        /// Flags all the data from the given antenna.
        /// Flags all the data from the given antenna \p a at channel \p c and
        /// polarisation \p p, with reason code \p reason.
        /// This is an alias for flag(a, c, p, reason) .
        void flagAntenna(const unsigned a, const unsigned c, const unsigned p,
                const unsigned char reason) {
            flag(a, c, p, reason);
        }

        /// Flags all antennas in the given channel.
        /// Flags all antennas in the given channel \p c at polarisation \p p,
        /// with reason code \p reason.
        /// This is an alias for flag(c, p, reason) .
        void flagChannel(const unsigned c, const unsigned p,
                const unsigned char reason) {
            flag(c, p, reason);
        }

        /// Returns a reference to the list of flagged antennas.
        /// This method returns a const-reference to the list of antennas
        /// flagged for the given channel \p c and polarisation \p.
        const std::vector<unsigned>& flaggedAntennas(unsigned c, unsigned p) const {
            return _flaggedAntennas[p][c];
        }

        /// Returns a pointer to the list of flagged antennas.
        /// This method returns a const pointer to the start of the list of
        /// flagged antennas for the given channel \p c and polarisation \p p.
        const unsigned* flaggedAntennaPtr(const unsigned c,
                const unsigned p) const {
            unsigned size = _flaggedAntennas[p][c].size();
            return (size > 0) ? &_flaggedAntennas[p][c][0] : NULL;
        }

        /// Returns a reference to the list of flagged channels.
        /// This method returns a const-reference to the list of channels
        /// flagged for the given polarisation \p p.
        const std::vector<unsigned>& flaggedChannels(const unsigned p) const {
            return _flaggedChannels[p];
        }

        /// Returns a pointer to the list of flagged channels.
        /// This method returns a const pointer to the start of the list of
        /// flagged channels for the given polarisation \p p.
        const unsigned* flaggedChannelPtr(const unsigned p) const {
            unsigned size = _flaggedChannels[p].size();
            return (size > 0) ? &_flaggedChannels[p][0] : NULL;
        }

        /// Returns the flag table entry.
        /// This method returns the flag table entry for the given antenna
        /// pair (\p ai, \p aj), channel \p c and polarisation \p.
        unsigned flags(const unsigned ai, const unsigned aj,
                const unsigned c, const unsigned p) const {
            return (*this)(ai, aj, c, p);
        }

        /// Returns the flag table entry.
        /// This method returns the flag table entry for the given antenna \p a,
        /// channel \p c and polarisation \p.
        unsigned flags(const unsigned a, const unsigned c,
                const unsigned p) const {
            return (*this)(a, a, c, p);
        }

        /// Returns the number of flagged antennas.
        /// This method returns the number of antennas flagged for the given
        /// channel \p c and polarisation \p.
        const unsigned nFlaggedAntennas(const unsigned c,
                const unsigned p) const {
            return _flaggedAntennas[p][c].size();
        }

        /// Returns number of flagged channels.
        /// This method returns the number of channels flagged for the given
        /// polarisation \p.
        const unsigned nFlaggedChannels(const unsigned p) const {
            return _flaggedChannels[p].size();
        }

        /// Resizes the flag table.
        /// Overrides (and calls) the base class method
        /// AntennaMatrixData<unsigned char>::resize() .
        ///
        /// @param[in] antennas The number of antennas in the visibility matrix.
        /// @param[in] channels The number of frequency channels.
        /// @param[in] polarisations The number of polarisations.
        void resize(const unsigned antennas, const unsigned channels,
                const unsigned polarisations) {
            _flaggedChannels.resize(polarisations);
            _flaggedAntennas.resize(polarisations);
            for (unsigned p = 0; p < polarisations; p++) {
                _flaggedAntennas[p].resize(channels);
            }
            AntennaMatrixData<unsigned char>::resize(antennas, channels, polarisations);
        }
};

} // namespace pelican

#endif // FLAGTABLE_H

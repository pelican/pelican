#ifndef ANTENNAPOSITIONS_H
#define ANTENNAPOSITIONS_H

#include "pelican/data/DataBlob.h"
#include <vector>

/**
 * @file AntennaPositions.h
 */

namespace pelican {

/**
 * @class AntennaPositions
 *
 * @brief
 * Container class to hold antenna positions.
 *
 * @details
 * This class holds antenna positions for a telescope.
 */
class AntennaPositions : public DataBlob
{
    public:
        /// Constructs an empty antenna positions data blob.
        AntennaPositions() : DataBlob() {}

        /// Constructs and assigns memory for an antenna positions data blob.
        AntennaPositions(const unsigned nAntennas) : DataBlob() {
            resize(nAntennas);
        }

    public:
        /// Clears the antenna positions data.
        void clear();

        /// Assign memory for antenna positions.
        void resize(const unsigned nAntennas);

    public: // accessor methods

        /// Returns the number of antennas for which positions are held.
        unsigned nAntennas() const { return _x.size(); }

        /// Returns a reference to the x coordinate antenna position for
        /// antenna \p i.
        real_t& x(const unsigned i) { return _x[i]; }

        /// Returns a reference to the y coordinate antenna position for
        // antenna \p i.
        real_t& y(const unsigned i) { return _y[i]; }

        /// Returns a reference to the z coordinate antenna position for
        /// antenna \p i.
        real_t& z(const unsigned i) { return _z[i]; }

        /// Returns a reference to the vector for the x antenna positions.
        std::vector<real_t>& x() { return _x; }

        /// Returns a reference to the vector for the y antenna positions.
        std::vector<real_t>& y() { return _y; }

        /// Returns a reference to the vector for the z antenna positions.
        std::vector<real_t>& z() { return _z; }

        /// Returns a pointer to the x antenna positions.
        real_t* xPtr() { return _x.size() > 0 ? &_x[0] : NULL; }

        /// Returns a pointer to the y antenna positions.
        real_t* yPtr() { return _y.size() > 0 ? &_y[0] : NULL; }

        /// Returns a pointer to the z antenna positions.
        real_t* zPtr() { return _z.size() > 0 ? &_z[0] : NULL; }

        /// Returns a pointer to the x antenna positions.
        const real_t* xPtr() const { return _x.size() > 0 ? &_x[0] : NULL; }

        /// Returns a pointer to the y antenna positions.
        const real_t* yPtr() const { return _y.size() > 0 ? &_y[0] : NULL; }

        /// Returns a pointer to the z antenna positions.
        const real_t* zPtr() const { return _z.size() > 0 ? &_z[0] : NULL; }

    private:
        std::vector<real_t> _x; ///< x coordinate of antenna position in metres.
        std::vector<real_t> _y; ///< y coordinate of antenna position in metres.
        std::vector<real_t> _z; ///< z coordinate of antenna position in metres.
};

} // namespace pelican

#endif // ANTENNAPOSITIONS_H

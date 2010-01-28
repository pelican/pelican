#ifndef ANTENNAPOSITIONSDATA_H
#define ANTENNAPOSITIONSDATA_H

#include <vector>

/**
 * @file AntennaPositionsData.h
 */

namespace pelican {

/**
 * @class AntennaPositions
 *  
 * @brief
 * Antenna positions container class.
 * 
 * @details
 * This class holds antenna positions for a telescope.
 */
class AntennaPositions : public DataBlob
{
    public:
        /// Constructor
        AntennaPositions();

        /// Constructor assigning memory for the antenna positions
        AntennaPositions(const unsigned int nAntennas);

        /// Image data destructor.
        ~AntennaPositions();

    public:
        /// Assign memory for antenna positions
        void assign(const unsigned int nAntennas);

        /// Clears the antenna positions data
        void clear();

    public: // accessor methods

        /// Returns the number of antennas for which positions are held
        int nAntennas() const { return _x.size(); }

        /// Returns the x coordinate antenna position for antenna i
        real_t& x(const unsigned int i) const { return _x[i]; }

        /// Sets the x coordinate of the antenna position for antenna i to the value specified
        void setX(const unsigned int i, const real_t value) { _x[i] = value; }

        /// Returns the y coordinate antenna position for antenna i
        real_t& y(const unsigned int i) const { return _y[i]; }

        /// Sets the y coordinate of the antenna position for antenna i to the value specified
        void setY(const unsigned int i, const real_t value) { _y[i] = value; }

        /// Returns the z coordinate antenna position for antenna i
        real_t& z(const unsigned int i) const { return _z[i]; }

        /// Sets the z coordinate of the antenna position for antenna i to the value specified
        void setZ(const unsigned int i, const real_t value) { _z[i] = value; }

        /// Returns a pointer to the x antenna positions.
        real_t* xPtr() { return _x.size() > 0 ? &_x[0] : NULL; }

        /// Returns a pointer to the y antenna positions.
        real_t* yPtr() { return _y.size() > 0 ? &_y[0] : NULL; }

        /// Returns a pointer to the z antenna positions.
        real_t* zPtr() { return _z.size() > 0 ? &_z[0] : NULL; }

    private:
        /// x coordinate of antenna position in metres.
        std::vector<real_t> _x;
        /// y coordinate of antenna position in metres.
        std::vector<real_t> _y;
        /// z coordinate of antenna position in metres.
        std::vector<real_t> _z;
};


} // namespace pelican

#endif // ANTENNAPOSITIONSDATA_H

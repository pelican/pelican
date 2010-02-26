#ifndef SOURCE_H_
#define SOURCE_H_

/**
 * @file source.h
 */

namespace pelican {


/**
 * @class Source
 *
 * @brief
 * Container class for a source.
 *
 * @details
 */

class Source
{
    public:
        enum { AZEL, J2000 };

    public:
        /// Constructs a source.
        Source(const double coord1 = 0.0, const double coord2 = 0.0,
                const double amp = 0.0, const unsigned coordType = J2000) {
            _coord1 = coord1;
            _coord2 = coord2;
            _amp = amp;
            _coordType = coordType;
        }

        /// Destroys the source.
        virtual ~Source() {}

    public:
        /// Returns the coordinate type (enum).
        unsigned& coordType() { return _coordType; }

        /// Returns the coordinate type (enum).
        unsigned coordType() const { return _coordType; }

        /// Returns the right ascension of the source.
        double& coord1() { return _coord1; }

        /// Returns the right ascension of the source.
        double coord1() const { return _coord1; }

        /// Returns the declination of the source.
        double& coord2() { return _coord2; }

        /// Returns the declination of the source.
        double coord2() const { return _coord2; }

        /// Returns the amplitude of the source.
        double& amp() { return _amp; }

        /// Returns the amplitude of the source.
        double amp() const { return _amp; }

    private:
        double _coord1;
        double _coord2;
        double _amp;
        unsigned _coordType;
};

} // namespace pelican

#endif /* SOURCE_H_ */

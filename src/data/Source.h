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
        typedef enum { AZEL, J2000 } coord_t;
        typedef enum { POL_XY } amp_t;

    public:
        /// Constructs a source.
        Source(const coord_t coordType = J2000, const double coord1 = 0.0,
                const double coord2 = 0.0, const amp_t ampType = POL_XY,
                const double amp1 = 0.0, const double amp2 = 0.0)
        {
            _coordType = coordType;
            _coord1 = coord1;
            _coord2 = coord2;
            _ampType = ampType;
            _amp1 = amp1;
            _amp2 = amp2;
        }

        /// Destroys the source.
        virtual ~Source() {}

    public:
        /// Returns the coordinate type (enum).
        coord_t& coordType() { return _coordType; }

        /// Returns the coordinate type (enum).
        coord_t coordType() const { return _coordType; }

        /// Returns the first coordinate of the source (RA or Az.).
        double& coord1() { return _coord1; }

        /// Returns the first coordinate of the source (RA or Az.).
        double coord1() const { return _coord1; }

        /// Returns the second coordinate of the source (Dec. or El.).
        double& coord2() { return _coord2; }

        /// Returns the second coordinate of the source (Dec. or El.).
        double coord2() const { return _coord2; }

        // Returns the amplitude type
        amp_t& ampType() { return _ampType; }

        // Returns the amplitude type
        amp_t ampType() const { return _ampType; }

        /// Returns the amplitude of the source. (X polarisation)
        double& amp1() { return _amp1; }

        /// Returns the amplitude of the source. (X polarisation)
        double amp1() const { return _amp1; }

        /// Returns the amplitude of the source. (Y polarisation)
        double& amp2() { return _amp2; }

        /// Returns the amplitude of the source. (Y polarisation)
        double amp2() const { return _amp2; }

    private:
        coord_t _coordType;
        double _coord1;
        double _coord2;
        amp_t _ampType;
        double _amp1;
        double _amp2;
};

} // namespace pelican

#endif /* SOURCE_H_ */

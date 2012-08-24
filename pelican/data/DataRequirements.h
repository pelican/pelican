#ifndef DATAREQUIREMENTS_H
#define DATAREQUIREMENTS_H


/**
 * @file DataRequirements.h
 */

#include <QtCore/QSet>
#include <QtCore/QString>

namespace pelican {

/**
 * @class DataRequirements
 *  
 * @brief
 * Specifies the types of data required by the pipeline modules
 * to operate a pipeline.
 *
 * @details
 * 
 */

class DataRequirements
{
    public:
        DataRequirements();
        ~DataRequirements();

        /// add a data requirement that must be met
        void addRequired( const QString& stream );
        void addRequired( const QSet<QString>& stream );

        /// add a data requirement that would be nice
        //  to have, but is not essential
        void addOptional( const QString& stream );
        void addOptional( const QSet<QString>& stream );

        /// Returns a set containing all required data types.
        QSet<QString> allStreams() const;

        /// Return true if the specified stream is included
        bool contains(const QString& stream) const;

        /// Clears the data requirements object.
        void clear();
        
        /// Returns the set of required streams.
        inline const QSet<QString>& required() const {return _requiredData;}

        /// Returns the set of optional streams.
        inline const QSet<QString>& optional() const {return _optionalData;}

        /// return the number of streams in the object
        int size() const;

        /// Compute a hash value for use with QHash.
        uint hash() const;

        /// Test for equality with another object.
        bool operator==(const DataRequirements&) const;

        /// Test for inequality with another object.
        bool operator!=(const DataRequirements&) const;

        /// Merge another requirements object with this one.
        DataRequirements& operator+=(const DataRequirements&);

        /// substract the streams in the list and return the remainder
        void remove( const QList<QString>& list );

    private:
        mutable uint _hash;
        QSet<QString> _requiredData;
        QSet<QString> _optionalData;
};

/// Compute a hash value for use with QHash (uses the hash member function).
uint qHash(const DataRequirements& key);

bool operator==(const DataRequirements&, const QList<QString>&);

} // namespace pelican
#endif // DATAREQUIREMENTS_H 

#ifndef COREOPTIONS_H
#define COREOPTIONS_H

#include <QString>

/**
 * @file CoreOptions.h
 */

/**
 * @class CoreOptions
 *  
 * @brief
 * 
 * @details
 * 
 */

namespace pelican {

class CoreOptions
{
    public:
        /// Constructor.
        CoreOptions();

        /// Constructor.
        CoreOptions(int argc, char** argv);

        /// Destructor.
        ~CoreOptions();

        /// Sets the type of data client required.
        void setDataClient(const QString& type);

        /// Returns the type of data client set.
        QString dataClient() const;

    private: /* Methods */
        
    private: /* Data */
        QString _dataClientType;
};

} // namespace pelican
#endif // COREOPTIONS_H 

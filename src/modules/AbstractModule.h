#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

class QDomElement;

/**
 * @file AbstractModule.h
 */

namespace pelican {

/**
 * @class AbstractModule
 *  
 * @brief
 *  base class for all modules
 * @details
 * 
 */

class AbstractModule
{
    public:
        AbstractModule( const QDomElement& config  );
        ~AbstractModule();

    private:
};

} // namespace pelican
#endif // ABSTRACTMODULE_H 

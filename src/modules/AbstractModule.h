#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

class QDomNode;

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
        AbstractModule( const QDomNode& config  );
        ~AbstractModule();

    private:
};

} // namespace pelican
#endif // ABSTRACTMODULE_H 

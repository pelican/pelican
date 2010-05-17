#ifndef FACTORYBASE_H
#define FACTORYBASE_H

#include <QString>
#include <vector>
#include <map>
#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"

/**
 * @file FactoryBase.h
 */

namespace pelican {

/**
 * @brief Factory base object.
 *
 * @details
 * This is the base class of all factories that produce abstract objects
 * of type B. The main function of the base class is to keep a record of all
 * objects created by the factory, so that they can be deleted on destruction.
 * The add() method is used to add the object to the list of objects created
 * by the factory.
 *
 * In the case of factories that create configurable objects, this
 * class can be constructed with a pointer to the configuration document.
 * Use the conf() method from within the factory to return the configuration
 * node for an object of a given type and name.
 */
template<class B> class FactoryBase
{
    public:
        /// Constructs the factory, setting the configuration object.
        FactoryBase(const Config* config = 0, const QString& section = "",
                const QString& group = "") :
                    _config(config), _section(section), _group(group) {}

        /// Destroys the factory and any objects created by it.
        virtual ~FactoryBase() {
            for (unsigned i = 0; i < _obs.size(); ++i) delete _obs[i];
        }

        /// Adds the given object to the list of those owned by the factory.
        B* add(B* ptr, const QString& type) {
            _obs.push_back(ptr);
            identifiers().insert(typename std::map<void*, QString>::
                    value_type((void*)_obs.back(), type));
            return _obs.back();
        }

        /// Return the configuration node for a type (named type if supplied).
        ConfigNode conf(const QString& type, const QString& name="") const {
            if (!_config) throw QString("Factory configuration not set");
            Config::TreeAddress address;
            address << Config::NodeId(_section, "");
            address << Config::NodeId(_group, "");
            address << Config::NodeId(type, name);
            return _config->get(address);
        }

        /// Returns the type of the allocated object.
        static const QString& whatIs(void* object) {
            if (identifiers().count(object) == 0)
                throw QString("Factory object identifier not known.");
            return identifiers()[object];
        }

    protected:
        /// Returns the map of objects created by all factories.
        static std::map<void*, QString>& identifiers() {
            static std::map<void*, QString> ids;
            return ids;
        }

    protected:
        const Config* _config; ///< Pointer to the configuration object.

    private:
        std::vector<B*> _obs;  ///< List of all objects created by the factory.
        QString _section;      ///< Configuration section (pipeline or server).
        QString _group;        ///< Configuration group name. (e.g. chunkers).
};

} // namespace pelican

#endif // FACTORYBASE_H

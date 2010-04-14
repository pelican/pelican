#ifndef FACTORY_H
#define FACTORY_H

#include <QString>
#include <QMap>

#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"

/**
 * @file Factory.h
 */

namespace pelican {

/**
 * @brief Abstract base for object creators.
 *
 * @details
 * This is a template class for generating object creators of a given base
 * class B (e.g. AbstractChunker or AbstractModule).
 */
template<class B> class AbstractCreator
{
    private:
        QString _type;

    public:
        /// Constructs an AbstractCreator.
        AbstractCreator(QString type) : _type(type) {}

        /// Creates an object of the given type (pure virtual).
        virtual B* create(const ConfigNode& config) = 0;

        /// Returns a reference to the type of this creator.
        const QString& type() {return _type;}

        /// Returns a reference to the static type map.
        static QMap<QString, AbstractCreator<B>*>& types() {
            static QMap<QString, AbstractCreator<B>*> types;
            return types;
        }
};

/**
 * @brief Template for creating objects.
 *
 * @details
 */
template<class T, class B> class Creator : public AbstractCreator<B>
{
    public:
        /// Constructs a Creator for the given type.
        /// This inserts the type name and a pointer to this generator
        /// into the map of known object types, so that it can be created.
        Creator(QString type) : AbstractCreator<B>(type) {
            AbstractCreator<B>::types().insert(type, this);
        }

        /// Allocates and constructs an object of the given type.
        /// This method constructs a concrete object of the required type, and
        /// returns its base class pointer.
        B* create(const ConfigNode& config) {return new T(config);}
};

/**
 * @class Factory
 *
 * @brief
 * Class to create configured objects.
 *
 * @details
 * This class creates configured objects that have been declared
 * using the PELICAN_DECLARE_* macros.
 * Calling the create() method will then generate the object with the
 * appropriate configuration.
 *
 * Use the PELICAN_DECLARE_* macro in the object's source (.cpp)
 * file to register the object with the factory.
 *
 * \code
 *
 * // Create the factory.
 * Config::TreeAddress address;
 * address.append(Config::NodeId("server", ""));
 * address.append(Config::NodeId("chunkers", ""));
 * Factory<AbstractChunker> factory(&config, address);
 *
 * // Create a chunker.
 * AbstractChunker* chunker = factory.create("MyChunkerType");
 *
 * \endcode
 */
template<class B> class Factory
{
    protected:
        const Config* _config;           ///< Configuration object pointer.
        Config::TreeAddress _configRoot; ///< Configuration root node.
        QList<B*> _objects;              ///< The list of constructed objects.

    public:
        /// Creates the factory.
        Factory(const Config* config, const Config::TreeAddress& base)
        : _config(config), _configRoot(base) {}

        /// Destroys the factory and deletes any objects created by it.
        virtual ~Factory() {
            for (int i = 0; i < _objects.size(); ++i)
                delete _objects[i];
        }

        /// Return the configuration node for a type (named type if supplied).
        ConfigNode configuration(const QString& type, const QString& name="") const
        {
            Config::TreeAddress address = _configRoot;
            address.append(Config::NodeId(type, name));
            return _config->get(address);
        }

        /// Creates a new configured object.
        /// Calling this method causes an object of the given \p type and
        /// \p name (default blank) to be allocated and configured
        /// appropriately.
        ///
        /// The method returns a base class pointer to the allocated object.
        virtual B* create(const QString& type, const QString& name = "")
        {
            if (AbstractCreator<B>::types().count(type) == 0)
                throw QString("Factory::create(): Unknown type '%1'").arg(type);

            B* object = AbstractCreator<B>::types()[type]->
                    create(configuration(type, name));
            _objects.append(object);
            return object;
        }
};

} // namespace pelican

#endif // FACTORY_H

#ifndef FACTORY_H
#define FACTORY_H

#include <QString>
//#include <QMap>
#include <string>
#include <map>
#include <iostream>

#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"

/**
 * @file Factory.h
 */

using std::map;
using std::string;

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
        string _type;
        static map<string, AbstractCreator<B>*> _types;

    public:
        /// Constructs an AbstractCreator.
        AbstractCreator(string type) : _type(type) {}

        /// Creates an object of the given type (pure virtual).
        virtual B* create(const ConfigNode& config) = 0;

        /// Returns a reference to the type of this creator.
        const string& type() {return _type;}

        /// Returns a reference to the type map.
        static map<string, AbstractCreator<B>*>& types() {return _types;}
};

template<class B> map<string, AbstractCreator<B>*> AbstractCreator<B>::_types;

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
        Creator(string type) : AbstractCreator<B>(type) {
//            AbstractCreator<B>::types().insert(type, this);
            std::cout << "Type: " << type << std::endl;
            std::pair<string, AbstractCreator<B>*> keyPair(type, this);
            std::cout << "Here" << std::endl;
            AbstractCreator<B>::types().insert(keyPair);
            std::cout << "Here" << std::endl;
        }

        /// Allocates and constructs an object of the given type.
        /// This method constructs a concrete object of the required type, and
        /// returns its base class pointer.
        B* create(const ConfigNode& config) {return new T(config);}
};

/**
 * @class ChunkerFactory
 *
 * @brief
 * Class to create configured chunker objects.
 *
 * @details
 * This class creates configured chunker objects that have been declared
 * using the PELICAN_DECLARE_CHUNKER macro.
 * Calling the create() method will then generate the object with the
 * appropriate configuration.
 *
 * Use the PELICAN_DECLARE_CHUNKER macro in the chunker's source (.cpp)
 * file to register the chunker with the factory.
 *
 * \code
 *
 * // Create the chunker factory.
 * Config::TreeAddress address;
 * address.append(Config::NodeId("server", ""));
 * address.append(Config::NodeId("chunkers", ""));
 * ChunkerFactory factory(&config, address);
 *
 * // Create a chunker.
 * AbstractChunker* chunker = factory.create("MyChunkerType");
 *
 * \endcode
 */
template<class B> class Factory
{
    private:
        QList<B*> _objects;

    protected:
        const Config* _config;
        Config::TreeAddress _configRoot;

    public:
        /// Creates the factory.
        Factory(const Config* config, const Config::TreeAddress& base)
        : _config(config), _configRoot(base) {}

        /// Destroys the factory and deletes any objects created by it.
        ~Factory() {
            for (int i = 0; i < _objects.size(); ++i)
                delete _objects[i];
        }

        /// Returns the tree node address that marks the start
        /// of the module configuration block.
        Config::TreeAddress configRoot() const { return _configRoot; }

        /// return the configuration node for a type (named type if supplied)
        ConfigNode configuration(const QString& type, const QString& name="") const
        {
            Config::TreeAddress address = _configRoot;
            address.append(QPair<QString, QString>(type, name));
            ConfigNode element = _config->get(address);
            return element;
        }


        /**
         * @details
         * Creates a new configured object of the given type, with the given
         * name (default blank).
         *
         * @param[in] type The object type.
         * @param[in] name The name of the object.
         *
         * @return Returns a pointer to the allocated object.
         */
        B* create(const QString& type, const QString& name)
        {
            string str = type.toStdString();
            if (AbstractCreator<B>::types().count(str) == 0)
                throw QString("Factory::create(): Unknown type '%1'").arg(type);

            B* object = AbstractCreator<B>::types()[str]->create(configuration(type, name));
            _objects.append(object);
            return object;
        }
};

} // namespace pelican

#endif // FACTORY_H

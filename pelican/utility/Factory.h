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

template<class B> class CreatorBase;

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
 * Factory<AbstractChunker> factory(&config, "server", "chunkers");
 *
 * // Create a chunker.
 * AbstractChunker* chunker = factory.create("MyChunkerType");
 *
 * \endcode
 */
template<class B> class Factory
{
    protected:
        const Config* _config; ///< Configuration object pointer.
        QString _section;      ///< Configuration section (pipeline or server).
        QString _group;        ///< Configuration group name. (e.g. chunkers).
        QList<B*> _objects;    ///< The list of constructed objects.

    public:
        /// Creates the factory.
        Factory(const Config* config = 0, const QString& section = "",
                const QString& group = "") :
                    _config(config), _section(section), _group(group) {}

        /// Destroys the factory and deletes any objects created by it.
        virtual ~Factory() {
            for (int i = 0; i < _objects.size(); ++i)
                delete _objects[i];
        }

        /// Return the configuration node for a type (named type if supplied).
        ConfigNode configuration(const QString& type, const QString& name="") const
        {
            if (!_config) throw QString("Factory::configuration(): Not set");
            Config::TreeAddress address;
            address.append(Config::NodeId(_section, ""));
            address.append(Config::NodeId(_group, ""));
            address.append(Config::NodeId(type, name));
            return _config->get(address);
        }

        /// Creates a new configured object.
        /// Calling this method causes an object of the given \p type and
        /// \p name (default blank) to be allocated and configured.
        /// It returns a base class pointer to the allocated object.
        virtual B* create(const QString& type, const QString& name = "")
        {
            if (CreatorBase<B>::types().count(type) == 0)
                throw QString("Factory::create(): Unknown type '%1'").arg(type);

            if (!_config)
                _objects.append(CreatorBase<B>::types()[type]->create());
            else
                _objects.append( CreatorBase<B>::types()[type]->
                        create(configuration(type, name)) );

            return _objects.last();
        }
};

/**
 * @brief Base class for object creators.
 *
 * @details
 * This is a template class for generating object creators of a given base
 * class B (e.g. AbstractChunker or AbstractModule).
 */
template<class B> class CreatorBase
{
    public:
        /// Creates an object of the required type.
        virtual B* create() {return 0;}

        /// Creates a configured object of the required type.
        virtual B* create(const ConfigNode&) {return 0;}

        /// Returns a reference to the static type map.
        static QMap<QString, CreatorBase<B>*>& types() {
            static QMap<QString, CreatorBase<B>*> types;
            return types;
        }
};

/**
 * @brief Template for creating basic objects.
 *
 * @details
 */
template<class T, class B> class CreatorBasic : public CreatorBase<B>
{
    public:
        /// Constructs a Creator for the required type.
        /// This inserts the type name and a pointer to this generator
        /// into the map of known object types, so that it can be created.
        CreatorBasic(QString type) : CreatorBase<B>() {
            CreatorBase<B>::types().insert(type, this);
        }

        /// Allocates and constructs an object of the required type.
        /// This method constructs a concrete object of the required type, and
        /// returns its base class pointer.
        B* create() {return new T;}

    private:
        /// Disallow other create() methods.
        B* create(const ConfigNode&) {return 0;}
};

/**
 * @brief Template for creating configured objects.
 *
 * @details
 */
template<class T, class B> class Creator : public CreatorBase<B>
{
    public:
        /// Constructs a Creator for the required type.
        /// This inserts the type name and a pointer to this generator
        /// into the map of known object types, so that it can be created.
        Creator(QString type) : CreatorBase<B>() {
            CreatorBase<B>::types().insert(type, this);
        }

        /// Allocates and constructs an object of the required type.
        /// This method constructs a concrete object of the required type,
        /// sets its configuration and returns its base class pointer.
        B* create(const ConfigNode& config) {return new T(config);}

    private:
        /// Disallow other create() methods.
        B* create() {return 0;}
};

} // namespace pelican

#endif // FACTORY_H

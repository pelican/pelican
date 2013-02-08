/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FACTORYBASE_H
#define FACTORYBASE_H

/**
 * @file FactoryBase.h
 */

#include "utility/Config.h"
#include "utility/ConfigNode.h"

#include <QtCore/QString>
#include <vector>
#include <map>

namespace pelican {

/**
 * @ingroup c_utility
 *
 * @brief Factory base object.
 *
 * @details
 * This is the base class of all factories that produce abstract objects
 * of type B. The main function of the base class is to keep a record of all
 * objects created by the factory, so that they can be optionally deleted
 * on destruction (this is the default). If the factory does not own the
 * objects, they will not be deleted by it.
 *
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
        /// Constructs the factory, setting the ownership flag.
        FactoryBase(bool owner = true) : _config(0), _owner(owner) {}

        /// Constructs the factory, setting the configuration object.
        FactoryBase(const Config* config, const QString& section,
                const QString& group, bool owner = true) :
                    _config(config), _owner(owner)
        {
            _base << Config::NodeId(section, "");
            _base << Config::NodeId(group, "");
        }

        /// Constructs the factory, setting the default configuration root.
        FactoryBase(const Config* config, const Config::TreeAddress& base,
                bool owner = true) :
            _config(config), _owner(owner), _base(base) {}

        /// Destroys the factory and any objects created by it.
        virtual ~FactoryBase() {
            if (_owner)
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
            Config::TreeAddress address(_base);
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
        const Config* _config;     ///< Pointer to the configuration object.

    private:
        bool _owner;               ///< Flag set if factory owns the objects.
        Config::TreeAddress _base; ///< Configuration root node.
        std::vector<B*> _obs;      ///< List of objects created by the factory.
};

} // namespace pelican

#endif // FACTORYBASE_H

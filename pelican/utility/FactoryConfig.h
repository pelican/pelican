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

#ifndef FACTORYCONFIG_H
#define FACTORYCONFIG_H

/**
 * @file FactoryConfig.h
 */

#include "utility/FactoryRegistrar.h"
#include "utility/FactoryBase.h"

#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>


namespace pelican {

/**
 * @ingroup c_utility
 *
 * @brief Blueprint for FactoryConfig.
 *
 * @details
 * This factory creates generic configured objects with base classes of type B.
 * In the public section of the base class declaration, there must be a
 * statement to define which parameter types must be given to all derived class
 * constructors. This is performed using the PELICAN_CONSTRUCT_TYPES(...)
 * macro, where the macro arguments are the list of types in the object's
 * constructor.
 *
 * So, for example, objects that take a double as well as the configuration node
 * in their constructor arguments would use
 *
 * @code
 * #include "utility/FactoryRegistrar.h"
 *
 * public:
 *     PELICAN_CONSTRUCT_TYPES(ConfigNode, double)
 * @endcode
 *
 * <b>For this factory to work, the first constructor argument must be of
 * type ConfigNode.</b>
 *
 * The factory can only create registered objects.
 * Use a PELICAN_DECLARE(BaseClassName, ObjectName) macro in the object's
 * header file to register the object with the factory.
 *
 * To create an object, call the create() method with the object's type ID
 * and all constructor arguments, <i>except</i> the first one
 * (the configuration node), which will be supplied by the factory.
 *
 * @code
 * Factory<AbstractChunker> factory(&config, "server", "chunkers");
 * AbstractChunker* chunker = factory.create("MyChunkerType");
 * @endcode
 */
template<class B,
    int = boost::mpl::size<typename B::FactoryCtorList>::value
> class FactoryConfig : public FactoryBase<B> {};

/**
 * This helper macro is only used by the FACTORYCONFIG macro.
 * It is used to obtain the types of the object constructor arguments after
 * the first one.
 */
#define PARAM1(z, n, data) \
typename boost::mpl::at_c<typename B::FactoryCtorList,BOOST_PP_INC(n)>::type P##n

/**
 * This macro is used to create multiple copies of FactoryConfig.
 */
#define FACTORYCONFIG(z, n, data) \
template<class B> class FactoryConfig<B, n> : public FactoryBase<B> { \
public: \
    /* Constructs a factory for creating configurable objects */ \
    FactoryConfig(const Config* config, const QString& section, \
            const QString& group, bool owner = true) : \
                FactoryBase<B>(config, section, group, owner) {} \
\
    /* Constructs a factory for creating configurable objects */ \
    FactoryConfig(const Config* config, const Config::TreeAddress& base, \
            bool owner = true) \
            : FactoryBase<B>(config, base, owner) {} \
\
    /* Creates and configures a concrete object with a registered ID */ \
    B* create(const QString& id BOOST_PP_ENUM_TRAILING(BOOST_PP_DEC(n),PARAM1,~) , \
            const QString& name="") { \
        RegBase<B, n>::check(id); \
        return this->add(RegBase<B, n>::types()[id]->create(FactoryBase<B>::conf(id, name) \
            BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_DEC(n), P) ), id); \
    } \
\
    /* configures an concrete object with a registered ID, inside pre-allocated memory */ \
    B* construct(B* memory, const QString& id BOOST_PP_ENUM_TRAILING(BOOST_PP_DEC(n),PARAM1,~) , \
            const QString& name="") { \
        RegBase<B, n>::check(id); \
        return this->add(RegBase<B, n>::types()[id]->construct(memory, FactoryBase<B>::conf(id, name) \
            BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_DEC(n), P) ), id); \
    } \
\
    /* Checks if the ID has been registered */ \
    bool exists(const QString& id) {return RegBase<B, n>::exists(id);} \
};
BOOST_PP_REPEAT(MAX_FACTORIES, FACTORYCONFIG, ~)
#undef FACTORYCONFIG
#undef PARAM1

} // namespace pelican

#endif // FACTORYCONFIG_H

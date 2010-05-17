#ifndef FACTORYGENERIC_H
#define FACTORYGENERIC_H

#include "pelican/utility/FactoryRegistrar.h"
#include "pelican/utility/FactoryBase.h"

#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

/**
 * @file FactoryGeneric.h
 */

namespace pelican {

/**
 * @brief Blueprint for FactoryGeneric.
 *
 * @details
 * This factory creates generic objects with base classes of type B.
 * In the public section of the base class declaration, there must be a
 * statement to define which parameter types must be given to all derived class
 * constructors. This is performed using the PELICAN_CONSTRUCT_TYPES(...)
 * macro, where the macro arguments are the list of types in the object's
 * constructor.
 *
 * So, for example, objects that take a double and a standard vector as
 * constructor arguments would use
 *
 * @code
 * PELICAN_CONSTRUCT_TYPES(double, std::vector<double>)
 * @endcode
 *
 * Objects that take no arguments must supply an empty list, using the
 * PELICAN_CONSTRUCT_TYPES_EMPTY macro.
 *
 * The factory can only create registered objects.
 * Use a PELICAN_DECLARE macro to register the object with the factory.
 *
 * To create an object, call the create() method with the object's type ID
 * and all constructor arguments.
 *
 * @code
 * FactoryGeneric<DataBlob> blobFactory;
 * blobFactory.create("VisibilityData");
 * @endcode
 */
template<class B,
    int = boost::mpl::size<typename B::FactoryCtorList>::value
> class FactoryGeneric : public FactoryBase<B> {};

/**
 * This macro is used to create multiple copies of FactoryGeneric.
 */
#define FACTORYGENERIC(z, n, data) \
template<class B> class FactoryGeneric<B, n> : public FactoryBase<B> { \
public: \
    /* Constructs a generic object factory */ \
    FactoryGeneric() : FactoryBase<B>() {} \
\
    /* Creates a concrete object with a registered ID */ \
    B* create(const QString& id BOOST_PP_ENUM_TRAILING(n, PARAM, ~)) { \
        RegBase<B, n>::check(id); \
        return add(RegBase<B, n>::types()[id]->create( \
                BOOST_PP_ENUM_PARAMS(n, P)), id); \
    } \
};
BOOST_PP_REPEAT(MAX_FACTORIES, FACTORYGENERIC, ~)
#undef FACTORYGENERIC

} // namespace pelican

#endif // FACTORYGENERIC_H
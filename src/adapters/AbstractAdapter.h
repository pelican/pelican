#ifndef ABSTRACTADAPTER_H
#define ABSTRACTADAPTER_H

#include <QDataStream>

/**
 * @file AbstractAdapter.h
 */

namespace pelican {

class ConfigNode;
class DataBlob;

/**
 * @class AbstractAdapter
 *  
 * @brief
 * Abstract base class for all data adapters.
 *
 * @details
 * This class is the base class for all Pelican data adapters, which
 * de-serialise data from an input stream.
 *
 * Inherit this class and implement the stream operator method to create a new
 * adapter.
 */

class AbstractAdapter
{
    public:
        /// Constructs a new adapter.
        AbstractAdapter(const ConfigNode& config);

        /// Destroys the adapter (virtual).
        virtual ~AbstractAdapter();

    public:
        /// Stream operator.
        //
        // see following links for reference:
        // http://www.parashift.com/c++-faq-lite/input-output.html#faq-15.8
        // http://www.learncpp.com/cpp-tutorial/93-overloading-the-io-operators/
        //
        // Create adapters when creating data client and link to data blob
        // =====================================================================
        // someAdapter = adapterFactory("SomeAdapter")
        // someAdapter.setData(visibilityData);
        //
        // getData() method then opens the file and reads from the file stream
        // into the adapter.
        // ====================================================================
        // QFile file("data.dat")
        // if (!file.open(QIODevice::ReadOnly)) { error... }
        // QDataStream in(&file);
        // in >> someAdapter;
        //
        friend QDataStream& operator>> (QDataStream& in, const AbstractAdapter& adapter);

    protected:
        /// Deseralise method to convert a stream into a data blob (pure virtual)
        virtual void deseralise(QDataStream& in) const = 0;

    protected:
        /// Data blob pointer to be by the adapter.
        DataBlob* _data;

        /// Chunk size in bytes.
        std::size_t _size;
};

} // namespace pelican

#endif // ABSTRACTADAPTER_H

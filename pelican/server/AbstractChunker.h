#ifndef ABSTRACTCHUNKER_H
#define ABSTRACTCHUNKER_H

#include "pelican/server/DataManager.h"
#include "pelican/server/WritableData.h"
#include "pelican/utility/Factory.h"

#include <QUdpSocket>
#include <QString>


/**
 * @file AbstractChunker.h
 */

namespace pelican {

/**
 * This macro is used to register the named chunker type with the factory.
 * It should be used within the global scope of the chunker's source file.
 *
 * @note
 * The macro expands to declare a dummy variable of the object's generator
 * type, which (when constructed), adds the type name to the creator's known
 * object types.
 *
 * The macro is placed within the global scope so that it is initialised as
 * soon as the program starts executing. It is placed within an anonymous
 * namespace so that the dummy creator variable is not accessible from outside
 * the file that instantiated it.
 */
#define PELICAN_DECLARE_CHUNKER(type) namespace {Creator<type, AbstractChunker> reg(#type);}


class ConfigNode;

/**
 * @class AbstractChunker
 *
 * @brief
 * Base class for all chunker plug-ins.
 *
 * @details
 * Methods on this class are called by the DataReceiver class
 * which sets up the necessary connections etc.
 */
class AbstractChunker
{
    private:
        DataManager* _dataManager;
        QString _type;
        QString _host;
        quint16 _port;

    protected:
        QIODevice* _device;

    public:
        /// Constructs a new AbstractChunker.
        AbstractChunker(const QString& type, QString host = "", quint16 port = 0)
            : _dataManager(NULL), _type(type), _host(host), _port(port), _device(NULL) {}

        AbstractChunker(const QString& type, const ConfigNode& config);

        AbstractChunker()
        : _dataManager(NULL), _type(""), _host(""), _port(0), _device(NULL)
        {}

        /// Destroys the AbstractChunker.
        virtual ~AbstractChunker();

        /// set the port to listen on for data
        void setPort(quint16 port);

        /// set the ip address to listen on for data
        void setHost(const QString& ipaddress);

        /// Sets th type name to be associated with this data.
        void setType(const QString& type) { _type = type; }

        /// Return the type name to be associated with this data.
        const QString& type() const {return _type;}

        /// Create a new device appropriate to the type expected on the data stream.
        virtual QIODevice* newDevice() = 0;

        /// Called whenever there is data ready to be processed.
        /// Inheriting classes should process a complete data chunk
        /// inside this method.
        virtual void next(QIODevice*) = 0;

        /// Returns the host.
        const QString& host() {return _host;}

        /// Returns the port.
        quint16 port() {return _port;}

        /// Sets the data manager.
        void setDataManager(DataManager* dataManager) {
            _dataManager = dataManager;
        }

    protected:
        /// Access to memory to store data is through this interface.
        /// The WritableData object should always be checked with its
        /// isValid() method before use. When the WritableData object goes
        /// out of scope the data will become available to be served
        /// automatically if it is valid.
        WritableData getDataStorage(size_t size) const;
};

} // namespace pelican

#endif // ABSTRACTCHUNKER_H

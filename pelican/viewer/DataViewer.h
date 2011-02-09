#ifndef DATAVIEWER_H
#define DATAVIEWER_H

/**
 * @file DataViewer.h
 */

#include "pelican/utility/Config.h"

#include <QtGui/QWidget>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QSet>

class QActionGroup;
class QTabWidget;
class QMenu;

namespace pelican {
class AbstractDataBlobClient;
class DataBlob;
class DataBlobWidget;
class DataBlobWidgetFactory;
class Stream;

/**
 * @ingroup c_viewer
 *
 * @class DataViewer
 *
 * @brief
 * Widget for Viewing Data Streams
 *
 * @details
 * This widget provides a GUI to connect to and listen to incomming data streams
 * Generally, you should create a suitable Widget for viewing a specific DataBlob
 * type. Use the setStreamViewer() method to associate this type with an incoming stream.
 *
 * \par Configuration example:
 * \code
 * <connection host="myTCPBlobServer" port="1234" />
 * \endcode
 */

class DataViewer : public QWidget
{
    Q_OBJECT

    public:
        DataViewer( const Config& config, const Config::TreeAddress& base, QWidget* parent=0 );
        virtual ~DataViewer();

        // returns the connected server host
        const QString& server() { return _server; };

        // returns the connected server port
        quint16 port() { return _port; };

        /// enable tracking of the specified stream
        //  n.b. call connectStreams() to actually
        //  connect to a stream after calling this
        //  to mark it for connection
        void enableStream( const QString& );

        /// disable tracking of the specified stream
        //  n.b. call connectStreams() to actually
        //  disconnect from a stream after calling this
        //  to mark it for deconnection
        void disableStream( const QString& );

        // flip the tracking status of the specifed stream
        // returns true if the new status is on false if off
        bool toggleStream( const QString& );

        // set internal state from the configuration file
        void setConfig( const ConfigNode& config);

        // set internal state from the configuration file
        void setClient( AbstractDataBlobClient& client );

        /// the default streams to listen to (on reset/init)
        //  true = default listen false = default inactive
        void setStreamDefault(const QString& stream, bool value);

        // set internal state from the configuration file
        void setStreamViewer( const QString& stream, const QString& viewerType);

        // returns a list of know streams
        QList<QString> streams() const;

    public slots:
        void dataUpdated(const QString& stream, DataBlob*);
        void dataUpdated(const Stream&);
        void newStreamData();
        void about();

    protected:
        virtual DataBlobWidget* getWidgetViewer(const QString& stream) const;
        void connectStreams( );
        void _updatedStreams( const QSet<QString>& streams );
        void closeEvent(QCloseEvent* event);

    protected slots:
        void _streamToggled();

    protected:
        DataBlobWidgetFactory* _viewerFactory;

    private:
        // GUI members
        QTabWidget* _streamTabs;
        QMenu* _viewMenu;
        QActionGroup* _streamActionGroup;

        AbstractDataBlobClient* _client;

        QMap<QString,int> _activeStreams; // currently activated streams and their tab id
        QMap<QString,bool> _defaultsEnabled; // the required state of a stream

        quint16 _port;
        QString _server;

        QMap<QString,QString> _streamViewerMap; // maps a stream name on to a type of viewer
};

} // namespace pelican
#endif // DATAVIEWER_H

#include "pelican/utility/WatchedFile.h"
#include <QtCore/QtGlobal>

namespace pelican {


/**
 * @details Constructs a WatchedFile object.
 */
WatchedFile::WatchedFile(const QString& name, QObject* parent)
    : QFile(name, parent)
{
    _init();
}

WatchedFile::WatchedFile(QObject* parent)
    : QFile( parent)
{
    _init();
}

void WatchedFile::_init()
{
    connect( &_watcher, SIGNAL( fileChanged(const QString&) ),
             this,  SLOT(_fileChanged(const QString &) ),
                                  Qt::DirectConnection);
    if( fileName() != QString() )
    {
        _watcher.addPath( fileName() );
    }
}

/**
 * @details Destroys the WatchedFile object.
 */
WatchedFile::~WatchedFile()
{
}

void WatchedFile::_fileChanged(const QString& )
{
    emit readyRead();
}

void WatchedFile::setFileName(const QString& name)
{
    if( fileName() != QString() )
        _watcher.removePath( fileName() );
    QFile::setFileName(name);
    _watcher.addPath( fileName() );
}

} // namespace pelican

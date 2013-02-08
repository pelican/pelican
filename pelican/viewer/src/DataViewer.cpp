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

#include <QtGui/QMessageBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QActionGroup>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QTabWidget>
#include <QtGui/QCloseEvent>

#include "viewer/DataViewer.h"
#include "viewer/DataBlobWidget.h"
#include "output/AbstractDataBlobClient.h"
#include "output/Stream.h"
#include "viewer/DataBlobWidgetFactory.h"
#include "utility/ConfigNode.h"

#include <iostream>

namespace pelican {

/**
 *@details DataViewer
 *    In development : ideally will use a generic Blob client and be exported to
 *    pelican
 */
DataViewer::DataViewer(const Config& config, const Config::TreeAddress& base, QWidget* parent)
    : QWidget(parent), _client(0)
{
    // setup our factory
    _viewerFactory = new DataBlobWidgetFactory(&config, base);

    // setup generic widgets
    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _streamTabs = new QTabWidget;

    // setup actions
    QMenuBar* menubar = new QMenuBar;

    // Main Menu
    QMenu* mainMenu = new QMenu(tr("Main"));
    QAction* exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    mainMenu->addAction(exitAct);
    menubar->addMenu(mainMenu);

    // View Menu
    _viewMenu = new QMenu(tr("View"));
    _viewMenu->setTearOffEnabled(true);
    menubar->addMenu(_viewMenu);
    _streamActionGroup = new QActionGroup(this);
    _streamActionGroup->setExclusive(false);
    _viewMenu->addSeparator();

    // Help Menu
    QMenu* helpMenu = new QMenu(tr("Help"));
    QAction* aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    helpMenu->addAction(aboutAct);
    menubar->addMenu(helpMenu);

    // set the layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(menubar);
    layout->addWidget(_streamTabs);
    //layout->addWidget(bottomFiller);
    setLayout(layout);

    setConfig(config.get(base));
}

/**
 *@details
 */
DataViewer::~DataViewer()
{
    delete _viewerFactory;
    //delete _streamActionGroup;
    //delete _viewMenu;
    //delete _streamTabs;
}

void DataViewer::setStreamViewer( const QString& stream, const QString& viewerType)
{
    _streamViewerMap[stream] = viewerType;
}

DataBlobWidget* DataViewer::getWidgetViewer(const QString& stream) const
{
    DataBlobWidget* widget = (DataBlobWidget*)0;
    try {
        if( _streamViewerMap.contains(stream) ) {
            widget = _viewerFactory->create( _streamViewerMap[stream] );
        }
        else
        {
            widget = _viewerFactory->create( stream );
        }
    }
    catch( const QString& )
    {
        // default is the basic datablob information Widget
        widget = _viewerFactory->create( "DataBlobWidget" );
        //widget = new SubbandSpectrumWidget;
    }
    return widget;
}

void DataViewer::setConfig(const ConfigNode& config)
{
    // set basic connection information
    _port = (quint16)config.getOption("connection", "port").toInt();
    _server = config.getOption("connection", "host");

    // set stream activation defaults
    // read in from the configuration TODO
    // _defaultsEnabled[stream] = true;

}

void DataViewer::setStreamDefault(const QString& stream, bool value)
{
    _defaultsEnabled[stream] = value;
}

void DataViewer::newStreamData()
{
    _updatedStreams( _client->streams() );
}

/*
 * @details
 * updates the Gui to reflect the specified streams
 */
void DataViewer::_updatedStreams( const QSet<QString>& streams )
{
    // clean up the previous state
    foreach(QAction* action, _streamActionGroup->actions() )
    {
        _streamActionGroup->removeAction(action);
        _viewMenu->removeAction(action);
        delete action;
    }

    // set up the new streams
    foreach(const QString& stream, streams )
    {
        QAction* a = _streamActionGroup->addAction(stream);
        a->setCheckable(true);
        _viewMenu->addAction(a);
        connect( a, SIGNAL(triggered() ), this, SLOT(_streamToggled()) );
        if( _defaultsEnabled.contains(stream) )
        {
            a->setChecked(true);
            enableStream(stream);
        }
        else {
            _defaultsEnabled[stream] = false;
            a->setChecked(false);
        }

    }
    connectStreams();
}

QList<QString> DataViewer::streams() const
{
    QList<QString> list;
    foreach( const QAction* action, _streamActionGroup->actions() )
    {
        list.append( action->text() );
    }
    return list;
}

void DataViewer::setClient(AbstractDataBlobClient& client)
{
    _client = &client;
    connect(_client, SIGNAL( newStreamsAvailable() ),
            this, SLOT( newStreamData() ) );
    connect(_client, SIGNAL(newData(const Stream&)),
            this, SLOT(dataUpdated(const Stream& )) );
    newStreamData();
}

/**
 * @details
 */
void DataViewer::closeEvent(QCloseEvent* event)
{
    event->accept();
}


void DataViewer::about()
{
    QMessageBox::about(this, tr("Data Stream Viewer"),
            tr("Connects to Pelican data Streams "
                "and displays them."));
}

void DataViewer::connectStreams()
{
    _client->subscribe(QSet<QString>::fromList(_activeStreams.keys()));
}

void DataViewer::dataUpdated(const Stream& stream)
{
    const QString& name = stream.name();
    if(  _activeStreams.contains(name) )
    {
        static_cast<DataBlobWidget*>(_streamTabs->widget(_activeStreams[name]))->updateData(stream.data().get());
    }
}

void DataViewer::dataUpdated(const QString& stream, DataBlob* blob)
{
    if(  _activeStreams.contains(stream) )
        static_cast<DataBlobWidget*>(_streamTabs->widget(_activeStreams[stream]))->updateData(blob);
}

void DataViewer::enableStream( const QString& stream )
{
    if( ! _activeStreams.contains(stream) )
    {
        _defaultsEnabled[stream] = true;
        _activeStreams[stream] = _streamTabs->addTab( getWidgetViewer(stream), stream );
    }
}

void DataViewer::disableStream( const QString& stream )
{
    if(  _activeStreams.contains(stream) )
    {
        _streamTabs->removeTab( _activeStreams[stream] );
        _activeStreams.remove(stream);
        _defaultsEnabled[stream] = false;
    }
}

bool DataViewer::toggleStream( const QString& stream )
{
    if( _activeStreams.contains(stream) )
    {
        disableStream(stream);
        return false;
    }
    else {
        enableStream(stream);
        return true;
    }
}

void DataViewer::_streamToggled()
{
    QAction* action = static_cast<QAction*>(sender());
    if( action )
        toggleStream( action->text() );
}

} // namespace pelican

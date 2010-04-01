/*
 * pelicanserver
 * Main server executable
 */

#include <QCoreApplication>
#include "DataManager.h"
#include "PelicanServer.h"
#include "PelicanPortServer.h"
#include "comms/PelicanProtocol.h"
#include "utility/Config.h"

using namespace pelican;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    //DataManager dm;
    //PelicanPortServer s(new PelicanProtocol, &dm);
    //s.listen(QHostAddress::Any , 2000);
    Config config;
    PelicanServer s(&config);
    s.addProtocol(new PelicanProtocol, 2000);
    s.start();
    return app.exec();
}

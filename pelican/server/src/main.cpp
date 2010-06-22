/*
 * pelican server
 * Main server executable
 */

#include "pelican/server/PelicanServer.h"
#include "pelican/server/DataManager.h"
#include "pelican/server/PelicanPortServer.h"
#include "pelican/comms/PelicanProtocol.h"
#include "pelican/utility/Config.h"

#include <QtCore/QCoreApplication>

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

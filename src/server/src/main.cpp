/*
 * pelicanserver
 * Main server executable
 */

#include <QCoreApplication>
#include "PelicanServer.h"
#include "PelicanPortServer.h"
#include "PelicanProtocol.h"

using namespace pelican;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    PelicanPortServer s(new PelicanProtocol);
    s.listen(QHostAddress::Any , 2000);
    //PelicanServer s;
    //s.addProtocol(new PelicanProtocol, 2000);
    //s.start();
    return app.exec();
}

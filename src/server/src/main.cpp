/*
 * pelicanserver
 * Main server executable
 */

#include <QCoreApplication>
#include "PelicanServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    pelican::PelicanServer s;
    s.start();
    while(1) {}
}

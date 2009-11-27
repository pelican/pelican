#include "Session.h"

namespace pelican {


// class Session 
Session::Session(int socketDescriptor,QObject* parent)
    : QThread(parent)
{
}

Session::~Session()
{
}

} // namespace pelican

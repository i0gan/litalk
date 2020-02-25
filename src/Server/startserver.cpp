#include "startserver.h"

StartServer::StartServer(QObject *parent) : QObject(parent)
{

    bProcess = new BackProsess(this);
    //connect(bProcess, &BackProsess::BcakProsessSignal, this, &StartServer::dealChildThread);
    //bProcess->start();
}

void StartServer::dealChildThread(QString recv) {
    //qDebug() << "reacv signal!" << recv;
}

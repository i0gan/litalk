#ifndef STARTSERVER_H
#define STARTSERVER_H

#include <QObject>
#include <QThread>
#include "backprosess.h"
#include <QUdpSocket>
#include <QStringList>

class StartServer : public QObject
{
    Q_OBJECT
public:
    explicit StartServer(QObject *parent = 0);
    void udpInit();
    void SendDataToLogin(QString data, QString hIp, quint16 port);
    void keepConnected();
private:
    QThread *thread_create;
    BackProsess *bProcess;
signals:

public slots:
    void dealChildThread(QString);
};
#endif // STARTSERVER_H

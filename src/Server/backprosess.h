#ifndef BACKPROSESS_H
#define BACKPROSESS_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantList>
#include <QDateTime>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>
#include <QFile>
#include <QDataStream>
class BackProsess : public QObject
{
    Q_OBJECT
    enum loginType{
        requestLogin = 16,
        login_ok,
        login_no,
        requestSignup,
        signup_ok,
        signup_no
    };
    enum tcpSendType{
        binFile = 64,          //send file
        };
    enum udpSendType{ //define send msg type of udp
        updateip = 128,
        getProfile, //get global information of person
        getPersonList,     //get public person List
        getFriendsList,
        msgToGroup,        //send msg to group
        msgToPerson,       //send msg to person
        newone,   //new one
        offline,  // deal with offline
        modifyPassword,
        modifyPasswordOk,
        modifyPasswordNo,
        modifyBaseInfo,
        modifyBaseInfoOk,
        modifyBaseInfoNo,
        getGlobalUserInfo, // get global user information
        getGlobalUserInfoOk,
        getGlobalUserInfoNo,
        adduser, //add friend
        adduserOk,
        adduserNo,
        friendDialog //getFriendInfo when talk
        };
public:
    explicit BackProsess(QObject *parent = 0);
    ~BackProsess();
    void init();
    void connectDatabase();
    bool login(QString account ,QString password);
    bool signup(QString acc, QString pas, QString nickname , QString sex);
    void loginOrSignupResultSend(loginType type, QHostAddress cIp, quint16 cUport);
    void tcpInit();
    void udpInit();
    void updateBaseInfoForTcp(QString account, quint16 tport);
    void updateBaseInfoForUdp(QString account, QString cIp, quint16 cUport);
    void dealOffline(QString account);
    void sendBaseInfo(udpSendType type,QString account, QString tAcc);
    void sendFriendList(QString account);
    void sendMsgToGroup(QString account, QString msg, udpSendType type);
    void synLog(QString msg);
    void sendPersonList(QString account); //this is a group pserson list
    void modifyPas(QString acc, QString oPas, QString nPas);
    void modifyInfo(QString acc, QString nickname, QString signature);

    void addUser(QString uAcc, QString fAcc, QString msg); //user account and target account
    void pendingToDo(QString fAcc);
    void dealPendingToDo(udpSendType type, QString pid);

    void sendMsgToPerson(QString uAcc, QString fAcc, QString msg);
private:
    QSqlDatabase db;
    QString dealBack;
    QUdpSocket *udpLoginSoket;
    QUdpSocket *udpSocket;
    QTcpServer *tcpServer;
    quint16 tcpPort;
    quint16 udpPort;
    quint16 udpLoginPort;
    QTimer  *synDataToLocalTimer;
    QFile *logFile;
protected:

signals:
    void BcakProsessSignal(QString recv);
public slots:
    void dealNewConnection();
    void dealUdpRecv(); //tcp recv
    void dealLoginRecv();
};

#endif // BACKPROSESS_H

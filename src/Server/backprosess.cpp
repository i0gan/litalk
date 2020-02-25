#include "backprosess.h"
//#define DEBUG
BackProsess::BackProsess(QObject *parent) : QObject(parent)
{

    init();
}
BackProsess::~BackProsess() {

}
void BackProsess::init() {
    this->logFile = new QFile(this);
    logFile->setFileName("./lyxf.log");
    logFile->open(QIODevice::WriteOnly);

    connectDatabase();

    udpInit();

    //tcpInit();
    //    synDataToLocalTimer = new QTimer(this);
    //    synDataToLocalTimer->start(1000 * 60); //1 minite to syn data to local;
    //    connect(synDataToLocalTimer, &QTimer::timeout, this, &BackProsess::synBaseInfoToLocal);
}
void BackProsess::tcpInit() {
    this->tcpPort = 7892;
    tcpServer = new QTcpServer();
    tcpServer->listen(QHostAddress::Any, this->tcpPort);
    connect(tcpServer, &QTcpServer::newConnection, this, &BackProsess::dealNewConnection);
    //qDebug() << "init tcp server OK";
}
void BackProsess::udpInit() {
    this->udpLoginPort = 7890;
    this->udpPort = 7891;
    this->udpSocket = new QUdpSocket(this);
    this->udpLoginSoket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::Any, this->udpPort);
    udpLoginSoket->bind(QHostAddress::Any, this->udpLoginPort);
    connect(this->udpSocket, &QUdpSocket::readyRead, this, &BackProsess::dealUdpRecv);
    connect(this->udpLoginSoket, &QUdpSocket::readyRead, this, &BackProsess::dealLoginRecv); //deal login recv
}
void BackProsess::dealLoginRecv() {
    QHostAddress cIp; //client ip
    quint16 cUport; //clinet port
    QByteArray buf = QByteArray(1024, 0);
    this->udpLoginSoket->readDatagram(buf.data(), buf.size(), &cIp, &cUport);
    QString recv = buf;
    QStringList recvList = recv.split("\x02");
    if(recvList.size() < 3) return;
    int type = QString(recvList.at(0)).toInt();
    switch (type) {
    case requestLogin: {
        //account + password
        if(true == login(recvList.at(1), recvList.at(2))) {
            loginOrSignupResultSend(login_ok, cIp, cUport);
        }else {
            loginOrSignupResultSend(login_no, cIp, cUport);
        }
    } break;
    case requestSignup: {
        if(recvList.size() < 5) break;
        if(true == signup(recvList.at(1), recvList.at(2), recvList.at(3), recvList.at(4))) {
            loginOrSignupResultSend(signup_ok, cIp, cUport);
        }else {
            loginOrSignupResultSend(signup_no, cIp, cUport);
        }
    } break;
    default:
        break;
    }
}
void BackProsess::loginOrSignupResultSend(loginType type, QHostAddress cIp, quint16 cUport) {
    QString send = QString("%1\x02").arg(type); //send login or Signup result
    this->udpLoginSoket->writeDatagram(send.toUtf8().data(), cIp, cUport);
}
void BackProsess::dealNewConnection() {

    //    QTcpSocket *tcpSocket = new QTcpSocket();
    //    tcpSocket = tcpServer->nextPendingConnection();

    //    QString hIp = tcpSocket->peerAddress().toString(); //get clinet ip
    //    quint16 hPort = tcpSocket->peerPort();             //get clinet port
    //    QString acc;
    //    qDebug() << "new connection: ip:" << hIp << "hPort:" << hPort;
    //    connect(tcpSocket, &QTcpSocket::readyRead, [=](){
    //        QByteArray buf = tcpSocket->readAll(); //read data
    //        qDebug() << "recv: " << buf;

    //        QStringList dataList = QString(buf).split("\x01");
    //        if(dataList.size() < 2) return;
    //        int type = QString(dataList.at(0)).toInt();
    //        QString account = dataList.at(1);

    //        switch (type) {
    //        case baseInfo:
    //            updateBaseInfo(account, hPort);
    //            sendBaseInfo(tcpSocket ,account);
    //            break;
    //        case getInfo:
    //            break;
    //        case modifyInfo:
    //            break;
    //        case binFile:
    //            break;
    //        default:
    //            break;
    //        }
    //    });
    //    connect(tcpSocket, &QTcpSocket::disconnected, [=](){
    //       qDebug() << "offline ip" << hIp << "port:" << hPort;
    //       dealOffline(hIp, hPort);
    //       tcpSocket->disconnectFromHost();
    //       tcpSocket->close();
    //       tcpSocket->destroyed();
    //    });
}
void BackProsess::dealUdpRecv() {

    QByteArray data = QByteArray(4096, 0);
    quint16 uport;
    QHostAddress ip;
    udpSocket->readDatagram(data.data(), data.size(), &ip, &uport);
    QStringList dataList = QString(data).split("\x01");
    if(dataList.size() < 2) return;
    int type = QString(dataList.at(0)).toInt();
    QString account = dataList.at(1);
    qDebug() << "recv";
    switch (type) {
    case updateip: {
        qDebug() << "updateip";
        updateBaseInfoForUdp(account, ip.toString(), uport);
    } break;
    case getProfile: {
        updateBaseInfoForUdp(account, ip.toString(), uport);
        qDebug() << "ip: " << ip.toString() << "  uport:" << uport;
        sendBaseInfo(getProfile, account, "");
        pendingToDo(account);
        sendMsgToGroup(account, "", newone);
    } break;
    case getPersonList: {
        sendPersonList(account); //send person List
    } break;
    case getFriendsList: {
        sendFriendList(account);
    } break;
    case friendDialog: {
        qDebug() << "frienddialog" << "acc:" << account;
        if(dataList.size() < 3) return;
        sendBaseInfo(friendDialog, account, dataList.at(2));
    } break;
    case offline: {
        qDebug() <<"account: " << account <<  ".... offline";
        dealOffline(account); //offline
    } break;
    case msgToGroup: {
        QString msg = dataList.at(2);
        qDebug () << "account " << account << "msg: " << msg;
        sendMsgToGroup(account, msg, msgToGroup);
    } break;
    case msgToPerson: {
        if(dataList.size() < 4) return ;
        sendMsgToPerson(account, dataList.at(2), dataList.at(3));
    } break;
    case modifyPassword: {
        if(dataList.size() < 3) return;
        qDebug() << "modify password";
        modifyPas(account, dataList.at(2), dataList.at(3));

    } break;
    case modifyBaseInfo: {
        qDebug() << "modify info";
        if(dataList.size() < 3) return;
        modifyInfo(account, dataList.at(2), dataList.at(3));

    } break;
    case getGlobalUserInfo: {
        if(dataList.size() < 3) return ;
        qDebug() << "get global user info";
        sendBaseInfo(getGlobalUserInfo, account, dataList.at(2));
    } break;
    case adduser: {
        qDebug() << "OOOO";
        if(dataList.size() < 4) return;
        qDebug() << "uacc " << account << "add tacc" << dataList.at(2);
        addUser(account, dataList.at(2), dataList.at(3));
    } break;
    case adduserOk: {
        if(dataList.size() < 3) return;
        dealPendingToDo(adduserOk, dataList.at(2));
    } break;
    case adduserNo: {
        if(dataList.size() < 3) return;
        dealPendingToDo(adduserNo, dataList.at(2));
        break;
    }
    default:
        break;
    }
}
void BackProsess::connectDatabase() { //modifyed by yourself

    //qDebug() << "child thread id" << QThread::currentThreadId();
    //qDebug() << "seport sql drive:" << QSqlDatabase::drivers();
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
#ifdef DEBUG
    db.setUserName("root"); //logan
    db.setPassword("****");
    db.setDatabaseName("litalk"); //you must modify it
#else
    db.setUserName("logan");
    db.setPassword("****");
    db.setDatabaseName("lyxf");
#endif


    if(false == db.open()) {
        qDebug() << "connect database fail : " << db.lastError().text();
        exit(-1);
    }
}
bool BackProsess::login(QString account ,QString password) {
    if(password.isEmpty()) return false;
    QSqlQuery query;
    query.exec(QString("select account from users"));
    while(query.next()) {
        if(query.value("account").toString() == account) {
            query.exec(QString("select password from users where account = '%1';").arg(account));
            query.next();
            if(query.value("password").toString() == password) {
                query.exec(QString("update users set online = 1 where account = '%1';").arg(account));
                return true;
            }else {
                return false;
            }
        }
    }
    return false;
}
bool BackProsess::signup(QString acc, QString pas, QString nickname , QString sex) {
    QSqlQuery query;
    query.exec("select account from users");
    while(query.next()) { //find this account is exist
        if(query.value("account").toString() == acc) {
            return false;
        }
    }
    QString sql = QString("insert into users(nickname, account, password, gender) values('%1', '%2', '%3', %4);")
            .arg(nickname).arg(acc).arg(pas).arg(sex);
    qDebug() << "signup: " << sql;
    return query.exec(sql);
}
void BackProsess::updateBaseInfoForTcp(QString account, quint16 tport) {
    QString sql = QString("update users set tport = %1, "
                          "online = 1 where account = '%2';")
            .arg(tport).arg(account);
    QSqlQuery query;
    query.exec(sql);
}
void BackProsess::updateBaseInfoForUdp(QString account, QString cIp, quint16 cUport) {
    QString sql = QString("update users set ip = '%1', uport = %2 where account = '%3';").arg(cIp).arg(cUport).arg(account);
    QSqlQuery query;
    query.exec(sql);
}
void BackProsess::dealOffline(QString account) {
    QSqlQuery query;
    QString sql = QString("update users set lastLogin = '%1', online = 0 where account = '%2';")
            .arg(QDateTime::currentDateTime().toString()).arg(account);
    query.exec(sql);
}
void BackProsess::sendBaseInfo(udpSendType type,QString account, QString tAcc) {
    QSqlQuery query;
    QString send;
    QString ip;
    quint16 uport;
    QString sql;
    sql = QString("select ip, uport from users where account = '%1';").arg(account);
    query.exec(sql);
    if(query.next()) {
        ip = query.value("ip").toString();
        uport = query.value("uport").toInt();
    }else return;

    if(type == getProfile) {
        sql = QString("select nickname, gender, grade, signature from users where account = '%1';").arg(account);
        query.exec(sql);
        query.next();
        QString nickname = query.value("nickname").toString();
        int gender = query.value("gender").toInt();
        int grade = query.value("grade").toInt();
        QString signature = query.value("signature").toString();
        send = QString("%1\x01%2\x01%3\x01%4\x01%5\x01")
                .arg(getProfile).arg(nickname).arg(gender).arg(grade).arg(signature);
    }
    qDebug() << "fAcc: " << tAcc;
    if(type == getGlobalUserInfo || type == friendDialog) {
        sql = QString("select account from users");
        query.exec(sql);
        while(query.next()) {
            if(query.value("account").toString() == tAcc) {
                qDebug() << "search yes";
                sql = QString("select nickname, gender, grade, signature, online from users where account = '%1';").arg(tAcc);
                query.exec(sql);
                query.next();
                QString nickname = query.value("nickname").toString();
                int gender = query.value("gender").toInt();
                int grade = query.value("grade").toInt();
                QString signature = query.value("signature").toString();
                int online = query.value("online").toInt();
                if(type == getGlobalUserInfo){
                    send = QString("%1\x01%2\x01%3\x01%4\x01%5\x01%6\x01")
                            .arg(getGlobalUserInfoOk).arg(nickname).arg(gender).arg(grade).arg(signature).arg(online);

                }else if(type == friendDialog) { //send friend info
                    send = QString("%1\x01%2\x01%3\x01%4\x01%5\x01%6\x01")
                            .arg(friendDialog).arg(nickname).arg(gender).arg(grade).arg(signature).arg(online);
                }
                this->udpSocket->writeDatagram(send.toUtf8().data(), QHostAddress(ip), uport);
                return ;
            }
        }
        qDebug() << "search no";
        send = QString("%1\x01").arg(getGlobalUserInfoNo);
    }
    this->udpSocket->writeDatagram(send.toUtf8().data(), QHostAddress(ip), uport);
}
void BackProsess::sendMsgToGroup(QString account, QString msg, udpSendType type) { //send msg to group with udp
    QSqlQuery query;
    bool isHave = query.exec(QString("select nickname from users where account = '%1';").arg(account));
    if(false == isHave) return;
    query.next();
    QString nickname = query.value("nickname").toString();
    query.exec("select ip, uport from users;");
    switch (type) {
    case msgToGroup: {
        synLog(QString("account: %2 send %3 group ").arg(account).arg(msg));
        while(query.next()) { //
            QString ip = query.value("ip").toString();
            quint16 port = query.value("uport").toInt();
            QString send = QString("%1\x01%2\x01%3\x01").arg(msgToGroup).arg(nickname).arg(msg);
            udpSocket->writeDatagram(send.toUtf8().data(), QHostAddress(ip), port);
        }
    }   break;
    case newone: {
        while(query.next()) { //
            QString ip = query.value("ip").toString();
            quint16 port = query.value("uport").toInt();
            QString send = QString("%1\x01%2\x01").arg(newone).arg(nickname);
            udpSocket->writeDatagram(send.toUtf8().data(), QHostAddress(ip), port);
        }
    }
    default:
        break;
    }
}
void BackProsess::synLog(QString msg) {
    this->logFile->atEnd();
    this->logFile->write(QString("%1  %2\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                         .arg(msg).toUtf8().data());
    this->logFile->flush();
    //QFile
}
void BackProsess::sendPersonList(QString account) {
    QSqlQuery query;
    QString send;
    QString ip;
    quint16 uport;
    QString sql;
    sql = QString("select ip, uport from users where account = '%1';").arg(account);
    query.exec(sql);
    if(query.next()) {
        ip = query.value("ip").toString();
        uport = query.value("uport").toInt();
    }else return;

    send = QString("%1\x01").arg(getPersonList);
    query.exec(QString("select nickname, account from users;"));
    while(query.next()) {
        send += query.value("nickname").toString();
        send += "(";
        send += query.value("account").toString();
        send += ")";
        send += "\n";
    }
    this->udpSocket->writeDatagram(send.toUtf8().data(), QHostAddress(ip), uport); // send person list;
}
void BackProsess::modifyPas(QString acc, QString oPas, QString nPas) {

    qDebug() << "old pas" << oPas << "nPas" << nPas;
    QSqlQuery query;
    QString send;
    QString ip;
    QString sql;
    quint16 uport;
    sql = QString("select ip, uport from users where account = '%1';").arg(acc);
    query.exec(sql);
    if(query.next()) {
        ip = query.value("ip").toString();
        uport = query.value("uport").toInt();
    }else return;

    sql = QString("select password from users where account = '%1';").arg(acc);
    query.exec(sql);
    if(false == query.next()) return;
    QString pas = query.value("password").toString();
    if(pas != oPas) {
        send = QString("%1\x01").arg(modifyPasswordNo);
    }else {
        sql = QString("update users set password = '%1' where account = '%2';").arg(nPas).arg(acc);
        query.exec(sql);
        send = QString("%1\x01").arg(modifyPasswordOk);
    }
    this->udpSocket->writeDatagram(send.toUtf8().data(), QHostAddress(ip), uport);
}
void BackProsess::modifyInfo(QString acc, QString nickname, QString signature) {
    QSqlQuery query;
    QString send;
    QString ip;
    quint16 uport;
    QString sql;
    sql = QString("select ip, uport from users where account = '%1';").arg(acc);
    query.exec(sql);
    if(query.next()) {
        ip = query.value("ip").toString();
        uport = query.value("uport").toInt();
    }else return;

    sql = QString("update users set nickname = '%1', signature = '%2' where account = '%3';")
            .arg(nickname).arg(signature).arg(acc);
    bool isOk = query.exec(sql);
    if(false == isOk){
        send = QString("%1\x01").arg(modifyBaseInfoNo);
    }else {
        send = QString("%1\x01").arg(modifyBaseInfoOk);
    }
    //return ;
    this->udpSocket->writeDatagram(send.toUtf8().data(), QHostAddress(ip), uport);

}
void BackProsess::addUser(QString uAcc, QString fAcc, QString msg) {

    if(msg.size() > 240) return;
    QSqlQuery query;
    QString send;
    QString ip;
    quint16 uport;
    QString sql;
    sql = QString("select ip, uport from users where account = '%1';").arg(uAcc);
    query.exec(sql);
    if(query.next()) {
        ip = query.value("ip").toString();
        uport = query.value("uport").toInt();
    }else return;
    if(uAcc == fAcc) {
        send = QString("%1\x01%2").arg(adduserNo).arg(QString("%1 are already your friends..").arg(fAcc));
        this->udpSocket->writeDatagram(send.toUtf8().data(), QHostAddress(ip), uport);
        return;
    }
    sql = QString("select uacc from user_friend;");
    query.exec(sql);
    while(query.next()) {
        if(fAcc == query.value("uacc").toString()) {
            send = QString("%1\x01%2").arg(adduserNo).arg(QString("%1 are already your friends..").arg(fAcc));
            this->udpSocket->writeDatagram(send.toUtf8().data(), QHostAddress(ip), uport);
            return;
        }
    }
    qDebug() << "uAcc: " << uAcc << "tAcc: " << fAcc << "msg" << msg;
    sql = QString("insert into pendingtodo (cmd, uacc, facc, msg) values(%1, %2, %3, '%4');")
            .arg(adduser).arg(uAcc).arg(fAcc).arg(msg);
    query.exec(sql); // create pendingtodo
    sql = QString("select online from users where account = %1;").arg(fAcc);
    query.exec(sql);
    query.next();
    int isOnline = query.value("online").toInt();
    if(isOnline == 1) {
        pendingToDo(fAcc);
    }
}
void BackProsess::pendingToDo(QString fAcc) {

    QSqlQuery query;
    QString sql = QString("select ip, uport from users where account = %1;").arg(fAcc);
    query.exec(sql);
    query.next();
    QString ip = query.value("ip").toString();
    quint16 uport = query.value("uport").toInt();
    qDebug() << "pendinToDo ip:" << ip << "  uport" << uport;
    sql = QString("select id, cmd, uacc, msg from pendingtodo where facc = %1").arg(fAcc);
    query.exec(sql);
    while(query.next()) {
        int pid = query.value("id").toInt();
        int cmd = query.value("cmd").toInt();
        int uacc = query.value("uacc").toInt();
        QString msg = query.value("msg").toString();
        QString send = QString("%1\x01%2\x01%3\x01%4\x01").arg(cmd).arg(uacc).arg(pid).arg(msg);
        qDebug() << "send::::: " << send.toUtf8();
        this->udpSocket->writeDatagram(send.toUtf8().data(), QHostAddress(ip), uport);
    }
}
void BackProsess::sendFriendList(QString account) {
    QSqlQuery query;
    QString send;
    QString ip;
    quint16 uport;
    QString sql;
    sql = QString("select ip, uport from users where account = '%1';").arg(account);
    query.exec(sql);
    if(query.next()) {
        ip = query.value("ip").toString();
        uport = query.value("uport").toInt();
    }else return;

    send = QString("%1\x01").arg(getFriendsList);
    sql = QString("select facc, remark from user_friend where uacc = '%1';").arg(account);
    query.exec(sql);
    while(query.next()) {
        send += query.value("facc").toString();
        send += "\x03";
        send += query.value("remark").toString();
        send += "\x02";
    }
    qDebug() << "sendFriendList: " << send;
    this->udpSocket->writeDatagram(send.toUtf8().data(), QHostAddress(ip), uport);
}
void BackProsess::sendMsgToPerson(QString uAcc, QString fAcc, QString msg) {

    qDebug() << "send msg to personal " << uAcc << "send " << msg << "to " << fAcc;
    QSqlQuery query;
    QString sql = QString("select online, ip, uport from users where account = '%1';").arg(fAcc);
    query.exec(sql);
    if(query.next()) {

        int online = query.value("online").toInt();
        qDebug() << "online :: " << online;
        if(online == 1) {
            QString ip = query.value("ip").toString();
            quint16 uport = query.value("uport").toInt();
            QString send = QString("%1\x01%2\x01%3\x01").arg(msgToPerson).arg(uAcc).arg(msg);

            qDebug() << "ip :" << ip << "uport: " << uport;
            this->udpSocket->writeDatagram(send.toUtf8().data(), QHostAddress(ip), uport);
        }
    }
}
void BackProsess::dealPendingToDo(udpSendType type, QString pid) {
    qDebug() << " deal pending to do";
    QSqlQuery query;
    QString sql;
    QString ip;
    quint16 uport;
    int online;
    QString send;
    sql = QString("select uacc, facc from pendingtodo where id = %1").arg(pid);
    query.exec(sql);
    if(false == query.next()) return;
    QString uacc = query.value("uacc").toString();
    QString facc = query.value("facc").toString();
    qDebug() << "uacc: " << uacc << "facc: " << facc;
    sql = QString("select ip, uport, online from users where account = '%1';").arg(uacc);
    query.exec(sql);
    if(query.next()) {
        ip = query.value("ip").toString();
        uport = query.value("uport").toInt();
        online = query.value("online").toInt();
    }else {
        return;
    }
    if(adduserOk == type) {
        sql = QString("insert into user_friend(uacc, facc) values('%1', %2);")
                .arg(uacc).arg(facc);
        query.exec(sql);
        sql = QString("insert into user_friend(uacc, facc) values('%1', %2);")
                .arg(facc).arg(uacc);
        query.exec(sql);
        sql = QString("delete from pendingtodo where id = %1").arg(pid);
        query.exec(sql);
        send = QString("%1\x01%2").arg(adduserOk).arg(QString("%1 accepte you fro add friend").arg(facc));
        qDebug() << "uacc:" << uacc << "add " << facc << "as friend";
    }else if(adduserNo == type) {
        sql = QString("delete from pendingtodo where id = %1").arg(pid);
        query.exec(sql);
        send = QString("%1\x01%2").arg(adduserOk).arg(QString("%1 rejected you for add friend.").arg(facc));
    }
    qDebug() << "send : deal pendingtodo " << send;
    this->udpSocket->writeDatagram(send.toUtf8().data(), QHostAddress(ip), uport);
}

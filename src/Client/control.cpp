#include "control.h"
//#define DEBUG
Control::Control(QObject *parent) : QObject(parent)
{
    login = new Login();
    login->show();
    connect(login, SIGNAL(logined(QString, QString, quint16)), this, SLOT(dealLogined(QString, QString, quint16)));
}
Control::~Control() {
    delete(login);
    delete(profile);
    delete(talk);
}
void Control::afterInit() {
    profile = new Profile();
    this->timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(keepConnect()));
    timer->start(3000); //每3秒发送一次包
    connect(profile, SIGNAL(showTalkWindow()), this, SLOT(showTalk())); //显示talk
    connect(this->profile, SIGNAL(closeWindow()), this, SLOT(dealClose())); //窗口关闭处理
    connect(profile, SIGNAL(modifyPassword(QString,QString)), this, SLOT(modifyPas(QString,QString))); //修改密码
    connect(profile, SIGNAL(modifyBaseProfile(QString,QString)), this, SLOT(modifyBaseProfile(QString,QString)));
    connect(profile, SIGNAL(updateProfileSginal()), this, SLOT(getbaseInfo())); //登陆时,获取自己的用户信息
    connect(profile, SIGNAL(getUserInfoSignal(QString)), this, SLOT(getUserInfo(QString))); //搜索时,获取全局用户信息
    connect(profile, SIGNAL(addUserSignal(QString, QString)), this ,SLOT(addUser(QString, QString))); //添加好友函数
    connect(profile, SIGNAL(friendDialogSignal(int)), this, SLOT(personTalk(int))); //好友之间对话
    getbaseInfo(); //基本信息初始化
    updateFriendsList(); //跟新好友列表
    profile->show();
    talk = new Talk();
    connect(talk, SIGNAL(sendMsg(QString)), this, SLOT(sendMsgToGroup(QString)));
}
void Control::tcpInit() { //tcp 初始化
    //this->sIp = "103.210.237.53";

    this->sTPort = 7892;
    timer = new QTimer(this);
    tcpSocket = new QTcpSocket(this);
    connect(this->tcpSocket, SIGNAL(connected()), this, SLOT(dealTcpConnected()));
    connect(this->tcpSocket, SIGNAL(readyRead()), this, SLOT(TcpRecv()));
    tcpSocket->connectToHost(QHostAddress(this->sIp), this->sTPort); //使用tcp连接服务器
}
void Control::udpInit() {
#ifdef DEBUG
    this->sIp = "192.168.100.3";
#else
    this->sIp = "103.210.237.53";
#endif

    this->sUPort = 7891;
    this->udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::Any, this->lport + 1);
    connect(this->udpSocket, SIGNAL(readyRead()), this, SLOT(UdpRecv()));
}
//登陆成功后处理函数
void Control::dealLogined(QString acc, QString pas, quint16 lport) {
    this->account  = acc;
    this->password = pas;
    this->lport = lport;
    udpInit(); //udp 初始化
    afterInit(); //登陆后控件初始化
}
void Control::getbaseInfo() {
    sendMsgWithUdp(getProfile, "", ""); //获取自身基本信息
    sendMsgWithUdp(getPersonList, "", "");
}
void Control::dealTcpConnected() {
    qDebug() << "Server connected !";
}
void Control::TcpRecv() {

}
void Control::UdpRecv() {

    QByteArray recv = QByteArray(4096, 0);
    QHostAddress serverIp;
    quint16 serverPort;
    udpSocket->readDatagram(recv.data(), recv.size(), &serverIp, &serverPort);
     qDebug() << "recv: " << recv.data();


    QStringList dataList = QString(recv).split("\x01");
    if(dataList.size() < 2) return;
    int type = QString(dataList.at(0)).toInt();
    switch (type) {
    case getProfile: {
        if(dataList.size() < 5) return;
        QString nickname = dataList.at(1);
        QString gender = dataList.at(2);
        QString grade = dataList.at(3);
        QString signature = dataList.at(4);
        qDebug() << "get Profile OK nickName:" << nickname << "gender" << gender << "grade:" << grade;
        this->profile->updateProfile(this->account, nickname, gender.toInt(), grade.toInt(), signature);
       } break;
    case getPersonList: {
        QString personList = dataList.at(1);
        qDebug() << personList;
        this->talk->updatePersonList(personList);
    } break;
    case getFriendsList: {
        if(dataList.size() < 2) return;
        QString friendsList = dataList.at(1);
        setFriendsList(friendsList);
    } break;
    case msgToGroup: {
        if(dataList.size() < 3) break;
        QString nickname = dataList.at(1);
        QString msg = dataList.at(2);
        this->talk->getMsg(nickname, msg);
   } break;
    case msgToPerson: {
        qDebug() << "recv person msgKKKKKK";
        if(dataList.size() < 3) return ;
        QString acc = dataList.at(1);
        QString msg = dataList.at(2);
        qDebug() << "recv personal msg:" << msg;
        recvPersonalMsg(acc, msg);
    } break;
    case newone: {
        QString nickname = dataList.at(1);
        this->talk->newOne(nickname);
    } break;
    case modifyPasswordOk: {
        qDebug() << "modify ok";
        this->profile->recvModifyResult(mPasswordOk);
    } break;
    case modifyPasswordNo: {
        this->profile->recvModifyResult(mPasswordNo);
    } break;
    case modifyBaseInfoOk: {
        this->profile->recvModifyResult(mBaseInfoOk);
    } break;
    case modifyBaseInfoNo: {
        this->profile->recvModifyResult(mBaseInfoNo);
    } break;
    case getGlobalUserInfoOk: {
        qDebug() << "get user info";
        if(dataList.size() < 6) return;
        QString nickname = dataList.at(1);
        QString gender = dataList.at(2);
        QString grade = dataList.at(3);
        QString signature = dataList.at(4);
        QString online  = dataList.at(5);
        this->profile->getUserInfoResult(searchUserOk, nickname, gender, grade, signature, online);
    } break;
    case getGlobalUserInfoNo: {

        this->profile->getUserInfoResult(searchUserNo, "", "", "", "", "");
    } break;
    case adduser: {
        if(dataList.size() < 4) return;
        AddfriendDialog *afd = new AddfriendDialog(0, dataList.at(1), dataList.at(2), dataList.at(3));
        connect(afd, SIGNAL(sendResult(int,QString)), this, SLOT(sendResultOfAddFriend(int,QString)));
        afd->show();
    } break;
    case adduserOk: {
        if(dataList.size() < 2) return;
        updateFriendsList();
        this->profile->recvNewMsg(dataList.at(1));
    } break;
    case adduserNo: {
        this->profile->recvNewMsg(dataList.at(1));
    }
    case friendDialog: {
        if(dataList.size() < 6) return;
        if(true == isOpenThisFriendDialog(this->fAcc)) {//判断是否已经打开了对话框
            if(false == isShowThisFriendDialog(this->fAcc)) { //判断是否已经显示了对话框
                showPTalkDialog(this->fAcc); //如果为隐藏的话,就显示出来
            }
            return;
        }
        friendDialogCreate(this->fAcc, dataList.at(1), dataList.at(2), dataList.at(3), dataList.at(4), dataList.at(5));
        showPTalkDialog(this->fAcc); //显示对话框
    } break;
    default:
        return;
    }
}
void Control::sendMsgWithUdp(udpSendType type, QString msg, QString target) {
    QByteArray send;
    switch (type) {
    case updateip: {
        send = QString("%1\x01%2\x01").arg(updateip).arg(this->account).toUtf8().data();
    } break;
    case getProfile: { //获取自己个人信息
        send = QString("%1\x01%2\x01").arg(getProfile).arg(this->account).toUtf8().data();
      } break;
    case getPersonList: { //获取公共群人员列表
        send = QString("%1\x01%2\x01").arg(getPersonList).arg(this->account).toUtf8().data();
       } break;
    case offline: { //处理离线
        send = QString("%1\x01%2\x01").arg(offline).arg(this->account).toUtf8().data();
    } break;
    case getFriendsList: { //获取好友列表
        send = QString("%1\x01%2\x01").arg(getFriendsList).arg(this->account).toUtf8().data();
    } break;
    case friendDialog: { //好友对话请求
        send = QString("%1\x01%2\x01%3\x01").arg(friendDialog).arg(this->account).arg(msg).toUtf8().data();
    } break;
    case msgToPerson: { //发送消息给好友
        qDebug() << "start";
        send = QString("%1\x01%2\x01%3\x01%4\x01")
                .arg(msgToPerson).arg(this->account).arg(target).arg(msg).toUtf8().data();
        qDebug() << "sssssssend::: " << send;
    } break;
    case msgToGroup: {
        send = QString("%1\x01%2\x01%3\x01").arg(msgToGroup).arg(this->account).arg(msg).toUtf8().data();
    } break;
    case modifyPassword: {
        send = QString("%1\x01%2\x01%3\x01%4\0x01").arg(modifyPassword)
                .arg(this->account).arg(msg).arg(target).toUtf8().data(); //修改密码
    } break;
    case modifyBaseInfo: {
        send = QString("%1\x01%2\x01%3\x01%4\0x01").arg(modifyBaseInfo)
                .arg(this->account).arg(msg).arg(target).toUtf8().data(); //个人信息
        qDebug() << "target" << target;
    } break;
    case getGlobalUserInfo: {
        send = QString("%1\x01%2\x01%3\x01").arg(getGlobalUserInfo).arg(this->account).arg(msg).toUtf8().data();
    } break;
    case adduser: {
        send = QString("%1\x01%2\x01%3\x01%4\x01").arg(adduser).arg(this->account).arg(target).arg(msg).toUtf8().data();
    } break;
    case adduserOk: {
        send = QString("%1\x01%2\x01%3\x01").arg(adduserOk).arg(this->account).arg(msg).toUtf8().data();
    } break;
    case adduserNo: {
        send = QString("%1\x01%2\x01%3\x01").arg(adduserNo).arg(this->account).arg(msg).toUtf8().data();
    } break;
    default:
        return;
    }
    qDebug() << "udp send: " << send << "ip: " << sIp << "port:" << sUPort;
    this->udpSocket->writeDatagram(send,  QHostAddress(this->sIp), this->sUPort);
}
void Control::dealClose() {
//        tcpSocket->disconnectFromHost();
//        tcpSocket->close();
        sendMsgWithUdp(offline, "", "");
        udpSocket->disconnectFromHost();
        udpSocket->close();
        qDebug() << "eixt";
    exit(0);
}
void Control::showTalk() {
    this->talk->show();
}
void Control::sendMsgToGroup(QString msg) {
    this->sendMsgWithUdp(msgToGroup, msg, "");
}
void Control::modifyPas(QString oPassword, QString nPassword) {
    qDebug() << "od Passwor" << oPassword << " modify passs" << nPassword;
    this->sendMsgWithUdp(modifyPassword, oPassword, nPassword);
}
void Control::modifyBaseProfile(QString nickname, QString signature) {
    qDebug() << "modify profile nickname:" << nickname << "sig " << signature;
    this->sendMsgWithUdp(modifyBaseInfo, nickname, signature);
}
void Control::getUserInfo(QString tAcc) {
    qDebug() << "serach acc" << tAcc;
    sendMsgWithUdp(getGlobalUserInfo, tAcc, "");
}
void Control::addUser(QString account, QString msg) {
    qDebug() << "add user" << account;
    sendMsgWithUdp(adduser, msg, account);
}
void Control::updateFriendsList() {
    qDebug() << "getfriendsList" << account;
    sendMsgWithUdp(getFriendsList, "", "");
}
void Control::setFriendsList(QString friends) {
    qDebug() << "frends: recv:" << friends.toUtf8();
    QStringList friendList = friends.split("\x02");
    for(int i = 0; i < friendList.size() - 1; i++) {
        QStringList infoList = QString(friendList.at(i)).split("\x03");
        if(infoList.size() < 2) {
            qDebug() << "update friendList error";
            return ;
        }
        this->friendsAcc.push_back(infoList.at(0));
        this->friendsNic.push_back(infoList.at(1));
        this->profile->newFriend(QString("%1 (%2)").arg(infoList.at(1)).arg(infoList.at(0))); //增一个好友 nickname + (account)
    }
}
void Control::personTalk(int index) {
    this->fAcc = this->friendsAcc.at(index);
    qDebug() << this->friendsAcc.at(index);
    sendMsgWithUdp(friendDialog,  this->fAcc, ""); //获取当前好友信息
}
void Control::sendPersonMsg(QString account, QString msg) {
    sendMsgWithUdp(msgToPerson,  msg, account);
}
int Control::findIdWithAcc(QString acc) {
    for(int i = 0; i < this->fAccList_.size(); i++) {
        if(acc == fAccList_.at(i)) {
            return i;
        }
    }
    return -1;
}
bool Control::isOpenThisFriendDialog(QString acc) {
    int id = findIdWithAcc(acc);
    if(id == -1) return false;
    else if(id >= 0) {
        return true;
    }
}
bool Control::isShowThisFriendDialog(QString acc) {
    int id = findIdWithAcc(acc);
    if(id == -1) return false;
    return this->isShowDialogList_.at(id);
}
void Control::showPTalkDialog(QString acc) { //显示对话框
    qDebug() << "show dialog acc:" << acc;
    int id = findIdWithAcc(acc);
    if(-1 == id) return ;
    PTalk *p = fTalkDialogList_.at(id);
    p->show();
}
void Control::hidePTalkDialog(QString acc) { //隐藏对话框
    qDebug() << "close window acc:" << acc;
    int id = findIdWithAcc(acc);
    if(-1 == id) return ;
    isShowDialogList_[id] = false;
    PTalk *p = fTalkDialogList_.at(id);
    p->hide();
}
void Control::recvPersonalMsg(QString acc, QString msg) {
    if(true == isOpenThisFriendDialog(acc)) {
        qDebug() << "recv msg this is a open windows";
        int id = findIdWithAcc(acc);
        PTalk *p = this->fTalkDialogList_.at(id);
        qDebug() << "msg: " << msg;
        p->recvMsg(msg); //不管咋地,都显示在对话框中
        if(false == isShowThisFriendDialog(acc)) { //如果对话框还没显示, 那就再消息那通知一下
            this->profile->recvNewMsg(QString("%1 send a msg to you..").arg(acc));
        }
        return ;
    }else {
        this->profile->recvNewMsg(QString("%1 send a msg to you as follows:").arg(acc));
        this->profile->recvNewMsg(msg);
    }
}
void Control::friendDialogCreate(QString account ,QString nickname, QString gender, QString grade, QString signature, QString online) {
    PTalk *p;
    p = new PTalk(0, account, nickname, gender, grade, signature, online);
    fTalkDialogList_.push_back(p); //储存对话框
    isOpenDialogList_.push_back(true);
    isShowDialogList_.push_back(true);
    fAccList_.push_back(this->fAcc);
    connect(p, SIGNAL(sendMsgSginal(QString,QString)), this, SLOT(sendPersonMsg(QString,QString)));
    connect(p, SIGNAL(hideWindow(QString)), this, SLOT(hidePTalkDialog(QString)));
}
void Control::sendResultOfAddFriend(int result, QString pid) {
    if(true == result) {
        sendMsgWithUdp(adduserOk,pid, "");
        updateFriendsList();
    }else {
        sendMsgWithUdp(adduserNo,pid, "");
    }
}
void Control::keepConnect() {
    qDebug() << "keep";
    sendMsgWithUdp(updateip,"", "");
}

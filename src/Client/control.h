#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include "login.h"
#include "profile.h"
#include "talk.h"
#include "ptalk.h"
#include "addfrienddialog.h"
#include <QTcpSocket>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QDataStream>
#include <QMessageBox>
#include <QVector>
class Control : public QObject
{
    Q_OBJECT
    enum tcpSendType{     //定义TCP发送消息的类型
        baseFile = 64,  //从服务器下载获取基本的信息的文件
        binFile           //发文件
    };
    enum udpSendType{      //定义UDP发送消息的类型
        updateip = 128,     //时时刻刻跟新ip 和端口,保持网络能连接
        getProfile,         //获取用户信息
        getPersonList,     //获取公共人员列表
        getFriendsList,     //获取好友列表
        msgToGroup,        //发送消息到群里
        msgToPerson,       //发送消息到个人
        newone,            //新用户
        offline,           //离线处理
        modifyPassword,
        modifyPasswordOk,
        modifyPasswordNo,
        modifyBaseInfo,
        modifyBaseInfoOk,
        modifyBaseInfoNo,
        getGlobalUserInfo,  //获取全局用户信息
        getGlobalUserInfoOk, //获取成功
        getGlobalUserInfoNo, //获取失败
        adduser,              //添加好友
        adduserOk,           //成功添加好友
        adduserNo,            //添加好友失败
        friendDialog        //好友聊天基础
    };
public:
    explicit Control(QObject *parent = 0);
    ~Control();

    void afterInit();    //登陆后,控件初始化
    void tcpInit();
    void udpInit();
    void sendMsgWithUdp(udpSendType type, QString Msg, QString target);
    void updateFriendsList();
    void setFriendsList(QString friends);
    int  findIdWithAcc(QString acc); //在账号列表中找对话框的id号
    bool isOpenThisFriendDialog(QString acc); //是否已经打开了对话框
    bool isShowThisFriendDialog(QString acc); //判断是否已经显示了对话框
    void showPTalkDialog(QString acc);
    void recvPersonalMsg(QString acc, QString msg);
    void friendDialogCreate(QString account ,QString nickname, QString gender, QString grade, QString signature, QString online);
    int generateRandomInteger(int min, int max);
private:
    Login *login;
    Profile *profile;
    Talk *talk;
    QString account;
    QString password;
    quint16 lport;
    QTcpSocket *tcpSocket;
    QUdpSocket *udpSocket;
    QString sIp;
    quint16 sTPort;
    quint16 sUPort;
    //bool isConnected;
    QTimer *timer;
    QStringList friendsAcc; //好友账号列表
    QStringList friendsNic; //好友昵称列表
    QString fAcc;

    QVector <PTalk *> fTalkDialogList_; //储存对话框
    QVector <bool> isOpenDialogList_;
    QVector <bool> isShowDialogList_;
    QVector <QString> fAccList_; //储存开启对话框后的账号
signals:

public slots:
    void dealLogined(QString, QString, quint16);
    void dealTcpConnected();
    void UdpRecv(); //udp recv
    void TcpRecv();
    void dealClose();
    void showTalk();
    void sendMsgToGroup(QString);
    void modifyPas(QString oPassword, QString nPassword);
    void modifyBaseProfile(QString nickname, QString signature);
    void getbaseInfo(); //登陆后,信息初始
    void getUserInfo(QString tAcc); //获取所搜索的用户信息
    void addUser(QString account, QString msg);
    void personTalk(int index);
    void sendPersonMsg(QString account, QString msg);
    void hidePTalkDialog(QString acc); //当用户点击关闭时, 隐藏窗口
    void sendResultOfAddFriend(int result, QString pid);
    void keepConnect();
};
#endif // CONTROL_H

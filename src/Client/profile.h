#ifndef PROFILE_H
#define PROFILE_H

#include <QWidget>
#include <QUdpSocket>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QMovie>
#include <QTimer>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QModelIndex>
#include <QPixmap>
namespace Ui {
class Profile;
}
    enum modifyType{
        mPasswordOk,
        mPasswordNo,
        mBaseInfoOk,
        mBaseInfoNo
    };
    enum getInfoType {
        searchUserOk,
        searchUserNo,
        searchGroupOk,
        searchGroupNo,
    };
class Profile : public QWidget
{
    Q_OBJECT

public:
    explicit Profile(QWidget *parent = 0);
    ~Profile();
    void initData();
    void updateProfile(QString account , QString nickname, int gender, int grade, QString signature);
    void recvModifyResult(modifyType type);
    void getUserInfoResult(getInfoType ,QString nickname, QString gender, QString grade, QString signature, QString online);
    void newFriend(QString remark);
    void recvNewMsg(QString msg);

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
signals:
    void closeWindow(); //关闭窗口
    void showTalkWindow(); //显示对话窗口
    void updateProfileSginal();
    void modifyHeadImage(QImage headImgae); //修改头像
    void modifyBaseProfile(QString nickname, QString signature); //修改基本信息
    void modifyPassword(QString oPassword, QString nPassword); //修改密码
    void getUserInfoSignal(QString account);
    void addUserSignal(QString account, QString msg);
    void updateMsgSignal(QString account);
    void friendDialogSignal(int index);

private slots:
    void on_pushButtonClose_clicked();
    void on_pushButtonMin_clicked();
    void on_pushButtonEnter_clicked();
    void on_pushButtonModifyInfo_clicked();
    void on_pushButtonModifyOk_clicked();
    void on_pushButtonProfile_clicked();
    void on_pushButtonModifyPasswordOk_clicked();
    void on_pushButtonAddFriend_clicked();
    void on_pushButtonAddGroup_clicked();
    void on_pushButtonCreateGroup_clicked();
    void on_pushButtonAddUserOk_clicked();
    void on_pushButtonAddFriendSearch_clicked();
    void on_pushButtonAddGroupOk_clicked();
    void friendDialog(QModelIndex);
    void on_pushButtonModifySelectImage_clicked();


    void on_pushButtonEnterPublicGroup_clicked();

    void on_pushButtonModifyPas_clicked();

    void on_pushButtonCancelModifyInfo_clicked();

    void on_pushButtonCancelModifyPass_clicked();

private:
    Ui::Profile *ui;
    QPoint mousePos;
    bool isUserSearched;
};
#endif // PROFILE_H

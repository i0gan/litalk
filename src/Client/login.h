#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>
#include <QMessageBox>
#include <QTime>
namespace Ui {
class Login;

}

class Login : public QWidget
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

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    void initData();
    void uploadInfo(loginType type);
    void sendData(QByteArray Data); //发送数据
    void dealLogin(); //登陆成功后的处理
    int generateRandomInteger(int min, int max);
private:
    Ui::Login *ui;
    QPoint mousePos; //鼠标位置
    QUdpSocket *udpSck; //udp socket
   // QTcpSocket *tcpSck;
    QString sIp;
    qint16 sPort;
    qint16 lPort;
    QByteArray sendByteData;
    QTimer cTimer; //连接时间计时器
    int i; //时间计算

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
signals:
    void logined(QString acc, QString pas, quint16 port);
    void closeWindow();
private slots:
    void on_pushButtonSignup_clicked();
    void on_pushButtonClose_clicked();
    void on_pushButtonBack_clicked();
    void on_pushButtonLogin_clicked();
    void dealRecv();
    void RealSend();
    void on_pushButtonSignupOk_clicked();
};

#endif // LOGIN_H

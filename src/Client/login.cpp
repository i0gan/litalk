#include "login.h"
#include "ui_login.h"
#include <QPainter>
#include <QLineEdit>
//#define DEBUG
#define SERVERPORT 7890
Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    initData();
}
void Login::initData() {
    this->sendByteData = QByteArray(1024, 0);
    ui->pushButtonLogin->setShortcut(Qt::Key_Return);
#ifdef DEBUG
    this->sIp = "192.168.100.3";
#else
    this->sIp = "103.210.237.53";
#endif
    sPort = 7890;
    lPort = generateRandomInteger(7000, 8000);
    udpSck = new QUdpSocket();
    //bind socket
    udpSck->bind(QHostAddress::Any, lPort);
    connect(udpSck, SIGNAL(readyRead()), this, SLOT(dealRecv())); //udp 信号
    connect(&cTimer, SIGNAL(timeout()), this, SLOT(RealSend())); //计时器
    ui->stackedWidget->setCurrentWidget(ui->WidgtLogin); //切换到登陆页面
    //remove borders
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->pushButtonLogin->setStyleSheet("QPushButton{"
                                       "border-image: url(:/images/res/button_1-1.png);"
                                       "}"
                                       "QPushButton:hover {"
                                       "border-image: url(:/images/res/button_1-2.png);"
                                       "}"
                                       "QPushButton:pressed {"
                                       "border-image: url(:/images/res/button_1-3.png);"
                                       "}");

    ui->pushButtonSignup->setStyleSheet("QPushButton{"
                                        "border-image: url(:/images/res/button_2-1.png);"
                                        "}"
                                        "QPushButton:hover {"
                                        "border-image: url(:/images/res/button_2-2.png);"
                                        "}"
                                        "QPushButton:pressed {"
                                        "border-image: url(:/images/res/button_2-3.png);"
                                        "}");
    ui->pushButtonSignupOk->setStyleSheet("QPushButton{"
                                          "border-image: url(:/images/res/button_3-1.png);"
                                          "}"
                                          "QPushButton:hover {"
                                          "border-image: url(:/images/res/button_3-2.png);"
                                          "}"
                                          "QPushButton:pressed {"
                                          "border-image: url(:/images/res/button_3-3.png);"
                                          "}");
    ui->pushButtonClose->setStyleSheet("QPushButton{"
                                       "border-image: url(:/images/res/close.png);"
                                       "}"
                                       "QPushButton:hover {"
                                       "border-image: url(:/images/res/close2.png);"
                                       "}");
    ui->pushButtonBack->setStyleSheet("QPushButton{"
                                      "border-image: url(:/images/res/back.png);"
                                      "}"
                                      "QPushButton:hover {"
                                      "border-image: url(:/images/res/back2.png);"
                                      "}");

    //ui->lineEditAccount->setTextColor(Qt::red);
    ui->lineEditAccount->setStyleSheet("QLineEdit{ border-image: url(:/images/res/InputBg.png);}");
    ui->lineEditPassword->setStyleSheet("QLineEdit{ border-image: url(:/images/res/InputBg.png);}");
    ui->lineEditSignAccount->setStyleSheet("QLineEdit{ border-image: url(:/images/res/InputBg.png);}");
    ui->lineEditSignNickname->setStyleSheet("QLineEdit{ border-image: url(:/images/res/InputBg.png);}");
    ui->lineEditSignPasswd_1->setStyleSheet("QLineEdit{ border-image: url(:/images/res/InputBg.png);}");
    ui->lineEditSignPasswd_2->setStyleSheet("QLineEdit{ border-image: url(:/images/res/InputBg.png);}");
}
Login::~Login()
{
    delete ui;
    delete udpSck;
}
void Login::paintEvent(QPaintEvent *) { //画背景图
    QPainter painter(this);
    painter.drawPixmap(-120, 0, 640, 640, QPixmap(":/images/res/bg_1.png"));
}
void Login::on_pushButtonSignup_clicked()//注册按钮
{
    ui->stackedWidget->setCurrentWidget(ui->WidgetSignup);
}
void Login::on_pushButtonClose_clicked() //关闭窗口
{
    udpSck->close();
    this->close();
    exit(0);
}
void Login::mouseMoveEvent(QMouseEvent *mouse) {//鼠标移动事件
    this->move(mouse->globalPos() - mousePos);
}
void Login::mousePressEvent(QMouseEvent *mouse) {//鼠标按下事件
    if(mouse->button() == Qt::LeftButton) {
        mousePos = mouse->globalPos() - this->frameGeometry().topLeft();
    }
}
void Login::on_pushButtonBack_clicked()//返回按钮
{
    ui->stackedWidget->setCurrentWidget(ui->WidgtLogin);
}
void Login::on_pushButtonLogin_clicked() //登陆按钮
{
    QString acc = ui->lineEditAccount->text();
    QString pas = ui->lineEditPassword->text();
    if(acc.isEmpty() | pas.isEmpty()) {
        QMessageBox::information(this, "error", "账号或者密码空了? %_%");
        return;
    }else if(acc.size() >= 32 || pas.size() >= 32) {
        QMessageBox::information(this, "error", "老实交代,你是不是没注册? ..@_@");
    }
    uploadInfo(requestLogin);

}
void Login::uploadInfo(loginType type) {
    QByteArray data;
    switch (type) {
    case requestLogin: {
        QString acc = ui->lineEditAccount->text();
        QString pas = QByteArray(ui->lineEditPassword->text().toUtf8()).toBase64();
        data = QString("%1\x02%2\x02%3\x02").arg(type).arg(acc).arg(pas).toUtf8();
    } break;
    case requestSignup: {
        QString nickname = ui->lineEditSignNickname->text();
        int sex = ui->comboBoxSignSex->currentIndex();
        QString acc = ui->lineEditSignAccount->text();
        QString pas = QByteArray(ui->lineEditSignPasswd_2->text().toUtf8()).toBase64();
        data = QString("%1\x02%2\x02%3\x02%4\x02%5\x02").arg(type).arg(acc).arg(pas).arg(nickname).arg(sex).toUtf8();
    } break;
    default:
        return;
    }
    sendData(data);
}
void Login::dealRecv() { //处理收到的数据
    if(cTimer.isActive()) {
        cTimer.stop();
    }
    QByteArray buf = QByteArray(128, 0);
    QHostAddress sAddr;
    quint16 port;
    udpSck->readDatagram(buf.data(), buf.size(), &sAddr, &port);
    QString recv = buf;
    qDebug() << "Login recv" << recv;
    QStringList recvList = recv.split("\x02");
    if(recvList.size() < 1) return ;
    int type = recvList.at(0).toInt();
    switch (type) {
    case login_ok: {
        dealLogin();
    } break;
    case login_no: {
        QMessageBox::information(this, "Login failed", "Maybe the account does not exist or the password is wrong!");
    } break;
    case signup_ok: {
        QMessageBox::information(this, "Congreatulations!", "Successful registration");
        ui->stackedWidget->setCurrentWidget(ui->WidgtLogin);
    } break;
    case signup_no: {
        QMessageBox::information(this, "Registration failed", "Maybe the account has existed!");
    } break;
        //qDebug() << cmd;
    default:
        break;
    }
}
void Login::on_pushButtonSignupOk_clicked() //注册按钮
{

    QString nickName = ui->lineEditSignNickname->text();
    QString account = ui->lineEditSignAccount->text();
    QString password_1 = ui->lineEditSignPasswd_1->text();
    QString password_2 = ui->lineEditSignPasswd_2->text();

    if(nickName.size() < 4 || nickName.size() >= 32) {
        QMessageBox::information(this, "error", "昵称字数要在4位以上 32位以下..@_@");
        return ;
    }
    if(account.size() < 4 || account.size() >= 32) {
        QMessageBox::information(this, "error", "账号字数要在4位以上 32位以下..^_@");
        return ;
    }
    if(password_1.size() < 6 || password_1.size() >= 32) {
        QMessageBox::information(this, "error", "密码字数要在6位以上 32位以下..^_@");
        return ;
    }
    if(password_1 != password_2) {
        QMessageBox::information(this, "error","哎呀, 两次输入的密码不一致啊..^_^");
        return ;
    }
    uploadInfo(requestSignup);
}
//发送数据
void Login::sendData(QByteArray Data) {
    if(Data.size() > 1024) return;
    i = 0;
    this->sendByteData = Data;
    qDebug() << Data.data();
    if(false == cTimer.isActive()) {
        cTimer.start(2000);
    }
}
void Login::RealSend() { //定时器发送数据
    i ++;
    udpSck->writeDatagram(this->sendByteData.data(), QHostAddress(sIp), sPort);
    if(i > 5) {
        QMessageBox::information(this, "error", "connect timeout!");
        if(cTimer.isActive()) {
            cTimer.stop();
        }
        return;
    }
}
void Login::dealLogin() {
    this->udpSck->disconnectFromHost();
    this->udpSck->close();
    QString acc = ui->lineEditAccount->text();
    QString pas = QByteArray(ui->lineEditPassword->text().toUtf8()).toBase64();
    emit this->logined(acc, pas, this->lPort);
    this->close();
}
//生成随机数, 防止端口重复占用
int Login::generateRandomInteger(int min, int max) {
    Q_ASSERT(min < max);
    static bool seedStatus;
    if(!seedStatus) {
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
        seedStatus = true;
    }
    int nRandom = qrand() % (max - min);
    nRandom += min;
    return nRandom;
}

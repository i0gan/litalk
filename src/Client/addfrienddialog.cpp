#include "addfrienddialog.h"
#include "ui_addfrienddialog.h"

AddfriendDialog::AddfriendDialog(QWidget *parent, QString account , QString pid , QString msg) :
    QWidget(parent),
    ui(new Ui::AddfriendDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint); //去边框
    this->pid = pid;
    ui->textBrowser->setTextColor(Qt::green);
    ui->textBrowser->append(QString("Account: %1").arg(account));
    ui->textBrowser->append(QString("Remark: %1").arg(msg));
    ui->pushButtonAccept->setStyleSheet("QPushButton{"
                                        "border-image: url(:/images2/res/button_accept.png); }"
                                        "QPushButton:hover {"
                                        "border-image: url(:/images2/res/button_accept2.png); }");
    ui->pushButtonReject->setStyleSheet("QPushButton{"
                                        "border-image: url(:/images2/res/button_reject1.png); }"
                                        "QPushButton:hover {"
                                        "border-image: url(:/images2/res/button_reject2.png); }");
    ui->pushButtonClose->setStyleSheet("QPushButton{"
                                       "border-image: url(:/images/res/close3.png); }"
                                       "QPushButton:hover {"
                                       "border-image: url(:/images/res/close4.png); }");
    ui->textBrowser->setStyleSheet("QTextBrowser{ border-image: url(:/images3/res/talk_bg-2.png);}");

    ui->labelHead->setPixmap(QPixmap("./images/default.jpg")); //设置头像
    ui->labelHead->setScaledContents(true);
}
void AddfriendDialog::paintEvent(QPaintEvent *) { //背景处理
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width(), this->height(), QPixmap(":/images3/res/talk_bg3.jpg")); //画背景
}
void AddfriendDialog::mouseMoveEvent(QMouseEvent *mouse) {//鼠标移动事件
        this->move(mouse->globalPos() - mousePos);
}
void AddfriendDialog::mousePressEvent(QMouseEvent *mouse) {//鼠标按下事件
    if(mouse->button() == Qt::LeftButton) {
        mousePos = mouse->globalPos() - this->frameGeometry().topLeft();
    }
}
AddfriendDialog::~AddfriendDialog()
{
    delete ui;
}

void AddfriendDialog::on_pushButtonAccept_clicked()
{
    emit sendResult(true, this->pid);
    this->close();
}

void AddfriendDialog::on_pushButtonReject_clicked()
{
    emit sendResult(false, this->pid);
    this->close();
}

void AddfriendDialog::on_pushButtonClose_clicked()
{
    this->close();
}

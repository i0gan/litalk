#include "talk.h"
#include "ui_talk.h"

Talk::Talk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Talk)
{
    ui->setupUi(this);
    init();
}
Talk::~Talk()
{
    delete ui;
}
void Talk::init() {
    ui->textEditInput->installEventFilter(this); //安装事件过滤器
    this->chatRecord = new QFile(this);
    this->chatRecord->setFileName("./chat.rc");

    if(this->chatRecord->open(QIODevice::ReadWrite)) loadCharRecord(); //打开对话记录
    this->windowSize = QSize(this->width(), this->height()); //储存窗口的大小
    isWindowMax = false; //是否为最大化
    setWindowFlags(Qt::FramelessWindowHint); //去边框
    ui->textEditInput->setStyleSheet("QTextEdit{ border-image: url(:/images3/res/talk_bg-2.png);}");
    ui->textBrowser->setStyleSheet("QTextBrowser{ border-image: url(:/images3/res/talk_bg-2.png);}");
    ui->tableWidget->setStyleSheet("QTableWidget {border-image: url(:/images3/res/talk_bg-2.png);}");
    ui->toolButtonClose->setStyleSheet("QToolButton{"
                                       "border-image: url(:/images/res/close3.png);"
                                       "}"
                                       "QToolButton:hover {"
                                       "border-image: url(:/images/res/close4.png);"
                                       "}");
    ui->toolButtonMax->setStyleSheet("QToolButton{"
                                       "border-image: url(:/images/res/max2.png);"
                                       "}"
                                       "QToolButton:hover {"
                                       "border-image: url(:/images/res/max.png);"
                                       "}");
    ui->toolButtonMin->setStyleSheet("QToolButton{"
                                       "border-image: url(:/images/res/min.png);"
                                       "}"
                                       "QToolButton:hover {"
                                       "border-image: url(:/images/res/min2.png);"
                                       "}");
    ui->pushButtonSend->setStyleSheet("QPushButton{"
                                       "border-image: url(:/images3/res/button_send.png);"
                                       "}"
                                       "QPushButton:pressed {"
                                       "border-image: url(:/images3/res/button_send2.png);"
                                       "}");
}
void Talk::getMsg(QString nickname,QString msg) {
   ui->textBrowser->setTextColor(Qt::green);
   QString text = QString("%1  %2 say: %3").arg(QDateTime::currentDateTime().toString("hh:mm")).arg(nickname).arg(msg);
   ui->textBrowser->append(text);
   ui->textBrowser->moveCursor(QTextCursor::End);
   this->chatRecord->atEnd(); //在文本末尾添加消息
   this->chatRecord->write(QString("%1\n").arg(text).toUtf8().data()); //写入消息
   this->chatRecord->flush();
}
void Talk::paintEvent(QPaintEvent *) { //背景处理
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width(), this->height(), QPixmap(":/images3/res/talk_bg3.jpg")); //画背景
}
void Talk::mouseMoveEvent(QMouseEvent *mouse) {//鼠标移动事件
        this->move(mouse->globalPos() - mousePos);
}
void Talk::mousePressEvent(QMouseEvent *mouse) {//鼠标按下事件
    if(mouse->button() == Qt::LeftButton) {
        mousePos = mouse->globalPos() - this->frameGeometry().topLeft();
    }
}
void Talk::on_toolButtonClose_clicked()
{
    this->close();
    this->chatRecord->close();
   // exit(0);
}
void Talk::on_toolButtonMax_clicked()
{
    if(false == this->isWindowMax) {
        this->setWindowState(Qt::WindowMaximized);
        this->isWindowMax = true;
    }else {
        this->isWindowMax = false;
        this->setWindowState(Qt::WindowActive);
    }
}
void Talk::on_toolButtonMin_clicked()
{
        this->setWindowState(Qt::WindowMinimized);
}
void Talk::newOne(QString nickname) {
    ui->textBrowser->setTextColor(Qt::red);
    ui->textBrowser->append(QString("%1                %2  online...").arg(QDateTime::currentDateTime().toString("hh:mm")).arg(nickname));
}
void Talk::updatePersonList(QString personList) {
    QStringList list = personList.split("\n");
    for(int i = 0; i < list.size() - 1; i++) {
        QTableWidgetItem *user = new QTableWidgetItem(list.at(i));
        user->setTextColor(Qt::red);
        ui->tableWidget->insertRow(0);
        ui->tableWidget->setItem(0, 0, user);
    }
}
void Talk::on_pushButtonSend_clicked() { //发送
    QString msg = ui->textEditInput->toPlainText();
    if(msg.isEmpty()) return;
    qDebug() << "OK send:" << msg.toUtf8().data();
    emit sendMsg(msg);
    ui->textEditInput->clear();
}
bool Talk::eventFilter(QObject *obj, QEvent *e) { //事件过滤器实现,回车发送消息
    if(obj == ui->textEditInput) {
        if(e->type() == QEvent::KeyPress) {
            QKeyEvent *env = static_cast<QKeyEvent *>(e); //强制转换
            if(env->key() == Qt::Key_Enter || env->key() == Qt::Key_Return) {
                this->on_pushButtonSend_clicked();
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, e); //接着传递
}

void Talk::loadCharRecord() {
    QByteArray text = this->chatRecord->read(0x1000);
    ui->textBrowser->append(text.data());
}

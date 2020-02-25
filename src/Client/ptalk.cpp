#include "ptalk.h"
#include "ui_ptalk.h"

PTalk::PTalk(QWidget *parent, QString acc, QString nickname, QString gender, QString grade, QString signature, QString online) :
    QWidget(parent),
    ui(new Ui::PTalk)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint); //去边框
    this->account = acc;
    this->nickname = nickname;
    //设置标题
    QString Title = "<html><head/><body><p><span style= 'font-size:14pt; color:#00ff00;'>";
    Title += nickname;
    Title += "</span></p></body></html>";
    this->ui->labelTitle->setText(Title);
    this->gender = gender;
    this->ui->textEditInput->installEventFilter(this); //install fitler
    ui->textBrowserInfo->append(QString("ACC: %1").arg(acc));
    ui->textBrowserInfo->append(QString("Nickname: %1").arg(nickname));

    if(gender == "0") {
        ui->textBrowserInfo->append("Gender: male");
        ui->labelHead->setPixmap(QPixmap("./images/male.jpg"));
        ui->labelHead->setScaledContents(true);
    }else {
        ui->textBrowserInfo->append("Gender: female");
        ui->labelHead->setPixmap(QPixmap("./images/female.jpg"));
        ui->labelHead->setScaledContents(true);
    }

    ui->textBrowserInfo->append(QString("Grade: %1").arg(grade));
    if(online == "1") {
        ui->textBrowserInfo->append("Network: online");
    }else {
        ui->textBrowserInfo->append("Network: offline");
    }
    ui->textBrowserInfo->append(QString("Signature: %1").arg(signature));
    init();
}
PTalk::~PTalk()
{
    delete ui;
}
void PTalk::init() {
    ui->textBrowser->setStyleSheet("QTextBrowser{ border-image: url(:/images3/res/talk_bg-2.png);}");
    ui->textBrowserInfo->setStyleSheet("QTextBrowser{ border-image: url(:/images3/res/talk_bg-2.png);}");
    ui->textEditInput->setStyleSheet("QTextEdit { border-image: url(:/images3/res/talk_bg-2.png);}");
    ui->pushButtonSend->setStyleSheet("QPushButton{"
                                       "border-image: url(:/images3/res/button_send.png);"
                                       "}"
                                       "QPushButton:pressed {"
                                       "border-image: url(:/images3/res/button_send2.png);"
                                       "}");
    ui->pushButtonCloseWindow->setStyleSheet("QPushButton{"
                                       "border-image: url(:/images/res/close3.png);"
                                       "}"
                                       "QPushButton:hover {"
                                       "border-image: url(:/images/res/close4.png);"
                                       "}");
    ui->pushButtonMinWindow->setStyleSheet("QPushButton{"
                                     "border-image: url(:/images/res/min.png);"
                                     "}"
                                     "QPushButton:hover {"
                                     "border-image: url(:/images/res/min2.png);"
                                     "}");
}
bool PTalk::eventFilter(QObject *obj, QEvent *e) { //事件过滤器实现,回车发送消息
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
void PTalk::paintEvent(QPaintEvent *) { //背景处理
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width(), this->height(), QPixmap(":/images3/res/talk_bg2.png")); //画背景
}
void PTalk::mouseMoveEvent(QMouseEvent *mouse) {//鼠标移动事件
        this->move(mouse->globalPos() - mousePos);
}
void PTalk::mousePressEvent(QMouseEvent *mouse) {//鼠标按下事件
    if(mouse->button() == Qt::LeftButton) {
        mousePos = mouse->globalPos() - this->frameGeometry().topLeft();
    }
}
void PTalk::on_pushButtonSend_clicked()
{
    QString msg = ui->textEditInput->toPlainText();
    if(msg.isEmpty()) {
        return;
    }
    qDebug() << "send..." << msg;
    ui->textBrowser->setTextColor(Qt::green);
    ui->textBrowser->append(QString("%1 You say: %2").arg(QDateTime::currentDateTime().toString("hh:mm")).arg(msg));
    ui->textEditInput->clear();
    emit sendMsgSginal(this->account, msg);
}
void PTalk::recvMsg(QString msg) {
    QString text;
    if(gender == "1") {
       text = QString("%1 He say: %2").arg(QDateTime::currentDateTime().toString("hh:mm")).arg(msg);

    }else {
       text = QString("%1 She say: %2").arg(QDateTime::currentDateTime().toString("hh:mm")).arg(msg);
    }
    ui->textBrowser->setTextColor(Qt::red);
    ui->textBrowser->append(text);
    ui->textBrowser->moveCursor(QTextCursor::End);
}
void PTalk::on_pushButtonMinWindow_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}
void PTalk::on_pushButtonCloseWindow_clicked()
{
   emit hideWindow(this->account); //隐藏窗口
}

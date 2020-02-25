#include "profile.h"
#include "ui_profile.h"
int i = 0;
Profile::Profile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Profile)
{
    ui->setupUi(this);
    //this->account = acc;
    initData();
    qDebug() << "profile";
    //getProfile();
}
Profile::~Profile()
{
    delete ui;
}
void Profile::initData() {
    //remove borders
    isUserSearched = false; //搜索用户的结果初始化
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->pushButtonClose->setStyleSheet("QPushButton{"
                                       "border-image: url(:/images/res/close3.png);"
                                       "}"
                                       "QPushButton:hover {"
                                       "border-image: url(:/images/res/close4.png);"
                                       "}");
    ui->pushButtonMin->setStyleSheet("QPushButton{"
                                     "border-image: url(:/images/res/min.png);"
                                     "}"
                                     "QPushButton:hover {"
                                     "border-image: url(:/images/res/min2.png);"
                                     "}");
    ui->pushButtonEnter->setStyleSheet("QPushButton{"
                                       "border-image: url(:/images2/res/button_4-1.png);"
                                       "}"
                                       "QPushButton:hover {"
                                       "border-image: url(:/images2/res/button_4-2.png);"
                                       "}"
                                       "QPushButton:pressed {"
                                       "border-image: url(:/images2/res/button_4-3.png);"
                                       "}");
    ui->textBrowser->setStyleSheet("QTextBrowser { border-image: url(:/images3/res/talk_bg-2.png);}");
    ui->pushButtonModifyInfo->setStyleSheet("QPushButton{"
                                            "border-image: url(:/images2/res/settings.png);"
                                            "}"
                                            "QPushButton:hover {"
                                            "border-image: url(:/images2/res/settings2.png);"
                                            "}"
                                            "QPushButton:pressed {"
                                            "border-image: url(:/images2/res/settings3.png);"
                                            "}");
    ui->pushButtonProfile->setStyleSheet("QPushButton{"
                                         "border-image: url(:/images2/res/home.png);"
                                         "}"
                                         "QPushButton:hover {"
                                         "border-image: url(:/images2/res/home2.png);"
                                         "}"
                                         "QPushButton:pressed {"
                                         "border-image: url(:/images2/res/home3.png);"
                                         "}");
    ui->pushButtonAddFriend->setStyleSheet("QPushButton{"
                                           "border-image: url(:/images2/res/add_user.png);"
                                           "}"
                                           "QPushButton:hover {"
                                           "border-image: url(:/images2/res/add_user2.png);"
                                           "}"
                                           "QPushButton:pressed {"
                                           "border-image: url(:/images2/res/add_user3.png);"
                                           "}");
    ui->pushButtonAddGroup->setStyleSheet("QPushButton{"
                                          "border-image: url(:/images2/res/group_add.png);"
                                          "}"
                                          "QPushButton:hover {"
                                          "border-image: url(:/images2/res/group_add2.png);"
                                          "}"
                                          "QPushButton:pressed {"
                                          "border-image: url(:/images2/res/group_add3.png); }");
    ui->pushButtonCreateGroup->setStyleSheet("QPushButton{"
                                             "border-image: url(:/images2/res/group_create.png); }"
                                             "QPushButton:hover {"
                                             "border-image: url(:/images2/res/group_create2.png); }"
                                             "QPushButton:pressed {"
                                             "border-image: url(:/images2/res/group_create3.png); }");
    ui->pushButtonAddFriendSearch->setStyleSheet("QPushButton{"
                                                 "border-image: url(:/images2/res/search.png); }"
                                                 "QPushButton:hover {"
                                                 "border-image: url(:/images2/res/search2.png); }"
                                                 "QPushButton:pressed {"
                                                 "border-image: url(:/images2/res/search3.png); }");
    ui->pushButtonAddGroupSearch->setStyleSheet("QPushButton{"
                                                "border-image: url(:/images2/res/search.png); }"
                                                "QPushButton:hover {"
                                                "border-image: url(:/images2/res/search2.png); }"
                                                "QPushButton:pressed {"
                                                "border-image: url(:/images2/res/search3.png); }");
    ui->pushButtonEnterPublicGroup->setStyleSheet("QPushButton{"
                                                  "border-image: url(:/images2/res/button_group.png); }"
                                                  "QPushButton:hover {"
                                                  "border-image: url(:/images2/res/button_group2.png); }"
                                                  "QPushButton:pressed {"
                                                  "border-image: url(:/images2/res/button_group3.png); }");
    ui->pushButtonAddUserOk->setStyleSheet("QPushButton{"
                                           "border-image: url(:/images2/res/button_add.png); }"
                                           "QPushButton:hover {"
                                           "border-image: url(:/images2/res/button_add2.png); }"
                                           "QPushButton:pressed {"
                                           "border-image: url(:/images2/res/button_add3.png; }");
    ui->pushButtonCancelModifyInfo->setStyleSheet("QPushButton{"
                                                  "border-image: url(:/images2/res/button_Cancel.png); }"
                                                  "QPushButton:hover {"
                                                  "border-image: url(:/images2/res/button_Cancel2.png); }");
    ui->pushButtonCancelModifyPass->setStyleSheet("QPushButton{"
                                                  "border-image: url(:/images2/res/button_Cancel.png); }"
                                                  "QPushButton:hover {"
                                                  "border-image: url(:/images2/res/button_Cancel2.png); }");
    ui->pushButtonModifySelectImage->setStyleSheet("QPushButton{"
                                                   "border-image: url(:/images2/res/button_select.png); }"
                                                   "QPushButton:hover {"
                                                   "border-image: url(:/images2/res/button_select2.png); }"
                                                   "QPushButton:pressed {"
                                                   "border-image: url(:/images2/res/button_select3.png; }");
    ui->pushButtonAddGroupOk->setStyleSheet("QPushButton{"
                                            "border-image: url(:/images2/res/button_add.png); }"
                                            "QPushButton:hover {"
                                            "border-image: url(:/images2/res/button_add2.png); }"
                                            "QPushButton:pressed {"
                                            "border-image: url(:/images2/res/button_add3.png; }");
    ui->pushButtonModifyOk->setStyleSheet("QPushButton{"
                                            "border-image: url(:/images2/res/button_Ok.png); }"
                                            "QPushButton:hover {"
                                            "border-image: url(:/images2/res/button_Ok2.png); }");
    ui->pushButtonModifyPasswordOk->setStyleSheet("QPushButton{"
                                                  "border-image: url(:/images2/res/button_Ok.png); }"
                                                  "QPushButton:hover {"
                                                  "border-image: url(:/images2/res/button_Ok2.png); }");

    ui->pushButtonModifyPas->setStyleSheet("QPushButton{"
                                           "border-image: url(:/images2/res/button_modifyPassword.png); }"
                                           "QPushButton:hover {"
                                           "border-image: url(:/images2/res/button_modifyPassword2.png); }");


    ui->lineEditModifyNickname->setStyleSheet("QLineEdit{ border-image: url(:/images2/res/lineEdit_bg.png);}");
    ui->lineEditModifyOrignalPassword->setStyleSheet("QLineEdit{ border-image: url(:/images2/res/lineEdit_bg.png);}");
    ui->lineEditModifyPassword_1->setStyleSheet("QLineEdit{ border-image: url(:/images2/res/lineEdit_bg.png);}");
    ui->lineEditModifyPassword_2->setStyleSheet("QLineEdit{ border-image: url(:/images2/res/lineEdit_bg.png);}");
    ui->textEditModifySignature->setStyleSheet("QTextEdit { border-image: url(:/images2/res/lineEdit_bg.png);}");
    ui->lineEditAddGroup->setStyleSheet("QLineEdit{ border-image: url(:/images2/res/lineEdit_bg.png);}");
    ui->lineEditAddUser->setStyleSheet("QLineEdit{ border-image: url(:/images2/res/lineEdit_bg.png);}");
    ui->textBrowserSeachUserResult->setStyleSheet("QTextBrowser { border-image: url(:/images2/res/lineEdit_bg.png);}");
    ui->textBrowserMsg->setStyleSheet("QTextBrowser { border-image: url(:/images2/res/lineEdit_bg.png);}");
    ui->stackedWidget->setCurrentIndex(0);
    ui->treeWidget->setColumnCount(1); //好友列表
    ui->treeWidget->setHeaderHidden(true);
    connect(ui->treeWidget, SIGNAL(doubleClicked(QModelIndex)), this,  SLOT(friendDialog(QModelIndex)));
}
void Profile::paintEvent(QPaintEvent *) { //画背景图
    QPainter painter(this);
    painter.drawPixmap(0, 0, 340, 600, QPixmap(":/images/res/bg_2.jpg"));
}
void Profile::mouseMoveEvent(QMouseEvent *mouse) {//鼠标移动事件
    this->move(mouse->globalPos() - mousePos);
}
void Profile::mousePressEvent(QMouseEvent *mouse) {//鼠标按下事件
    if(mouse->button() == Qt::LeftButton) {
        mousePos = mouse->globalPos() - this->frameGeometry().topLeft();
    }
}
void Profile::on_pushButtonClose_clicked()
{
    if(QMessageBox::question(this, "Warning !", "Are you sure to exit ?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes){
        this->close();
        emit this->closeWindow();
    }
}
void Profile::updateProfile(QString account , QString nickname, int gender, int grade, QString signature) {
    QString nick = "<html><head/><body><p><span style=' font-size:18pt; color:#55ffff;'>";
    nick += QString("Hello: %1 ").arg(nickname);
    nick += "</span></p></body></html>";

    QString genderText;
    if(gender == 0) genderText = "Male";
    else genderText = "Female";
    QString gen = "<html><head/><body><p><span style= 'font-size:12pt; font-weight:600; color:#55ffff;'>";
    gen += QString("Gender: %1 ").arg(genderText);
    gen += "</span></p></body></html>";

    QString gra = "<html><head/><body><p><span style= 'font-size:12pt; font-weight:600; color:#55ffff;'>";
    gra += QString("Grade: %1 ").arg(grade);
    gra += "</span></p></body></html>";
    QString acc = "<html><head/><body><p><span style= 'font-size:12pt; font-weight:600; color:#55ffff;'>";
    acc += QString("Account: %1").arg(account);
    acc += "</span></p></body></html>";
    ui->textBrowser->setText(signature);
    ui->labelNickname->setText(nick);
    ui->labelGender->setText(gen);
    ui->labelGrade->setText(gra);
    ui->labelAccount->setText(acc);
    if(gender == 0) {
        ui->labelHeadImage->setPixmap(QPixmap("./images/male.jpg"));
    }else {
        ui->labelHeadImage->setPixmap(QPixmap("./images/female.jpg"));
    }
    ui->labelHeadImage->setScaledContents(true);
}
void Profile::getUserInfoResult(getInfoType type,QString nickname, QString gender, QString grade, QString signature, QString online) {
    ui->textBrowserSeachUserResult->clear();
    ui->textBrowserSeachUserResult->setTextColor(Qt::red);
    switch (type) {
    case searchUserOk: {
        isUserSearched = true;
        ui->textBrowserSeachUserResult->append(QString("Nickname: %1").arg(nickname));
        if(gender == "0") {
            ui->textBrowserSeachUserResult->append(QString("Gender: male"));
        }else {
            ui->textBrowserSeachUserResult->append(QString("Gender: female"));
        }
        ui->textBrowserSeachUserResult->append(QString("Grade: %1").arg(grade));
        if(online == "1") {
            ui->textBrowserSeachUserResult->append(QString("Network: online"));
        }else {
            ui->textBrowserSeachUserResult->append(QString("Network: offline"));
        }
        ui->textBrowserSeachUserResult->append(QString("Signature: %1").arg(signature));

    } break;
    case searchUserNo: {
        isUserSearched = false;
        ui->textBrowserSeachUserResult->append("404 Not found this asshole!");
    }
    default:
        break;
    }
}
void Profile::on_pushButtonMin_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}
void Profile::on_pushButtonEnter_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
void Profile::on_pushButtonModifyInfo_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void Profile::on_pushButtonModifyOk_clicked()
{
    QString nickname = ui->lineEditModifyNickname->text();
    QString signature = ui->textEditModifySignature->toPlainText();
    if(nickname.size() < 4 || nickname.size() >=32) {
        QMessageBox::information(this, "error", "昵称字数要在4位以上 32位以下..@_@");
        return;
    }else if(signature.isEmpty()) {
        QMessageBox::information(this, "error", "大侠,还没写点啥个签..@_@");
        return;
    }else if(signature.size() > 240) {
        QMessageBox::information(this, "error", "大侠,你废话真多,少说一点..^_^.");
    }
    emit modifyBaseProfile(nickname , signature);
}
void Profile::on_pushButtonProfile_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Profile::on_pushButtonModifyPasswordOk_clicked()
{
    QString password_1 = ui->lineEditModifyPassword_1->text();
    QString password_2 = ui->lineEditModifyPassword_2->text();
    if(password_1.length() < 6 || password_1.length() > 32) {
        QMessageBox::information(this, "warning", "密码字数要在4位以上 32位以下..@_@");
        return ;
    } else if(password_1 != password_2) {
        QMessageBox::information(this, "warning", "输入的两次密码不一致");
        return ;
    }
    //对密码进行加密
    QString oPassword = QByteArray(ui->lineEditModifyOrignalPassword->text().toUtf8()).toBase64();
    QString nPassword = QByteArray(ui->lineEditModifyPassword_2->text().toUtf8()).toBase64();
    emit modifyPassword(oPassword, nPassword);
}
void Profile::recvModifyResult(modifyType type) {
    switch (type) {
    case mPasswordOk: {
        QMessageBox::information(this, "Congratulations!", "密码修改成功");
    } break;
    case mPasswordNo: {
        QMessageBox::information(this, "errro", "输入原密码错误或其他原因导致密码修改失败!");
    } break;
    case mBaseInfoOk: {
        QMessageBox::information(this, "Congratulations!", "信息修改成功");
        emit updateProfileSginal();
        ui->lineEditModifyNickname->clear();
        ui->textEditModifySignature->clear();
        ui->stackedWidget->setCurrentIndex(0);
    } break;
    case mBaseInfoNo: {
        QMessageBox::information(this, "errro", "不知道什么鬼东西导致信息修改失败!,试试全英文");
    }
    default:
        break;
    }
}


void Profile::on_pushButtonAddFriend_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(4);
}
void Profile::on_pushButtonAddGroup_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(5);
}
void Profile::on_pushButtonCreateGroup_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(6);
}
void Profile::on_pushButtonAddFriendSearch_clicked()
{
    QString acc = ui->lineEditAddUser->text();
    if(acc.isEmpty()) {
        QMessageBox::information(this, "error", "你还没输入呢..");
        return;
    }else if(acc.size() < 4 || acc.size() >= 32){
        QMessageBox::information(this, "error", "账号的字数不符合要求哦, 字数位: 4 ~ 32位之间..");
        return;
    }
    emit getUserInfoSignal(acc);
}
void Profile::on_pushButtonAddUserOk_clicked()
{
    if(this->isUserSearched) {
        emit addUserSignal(ui->lineEditAddUser->text(), "We are firend!!");
        isUserSearched = false;
        QMessageBox::information(this, "OK", "发送成功,等待好友接受吧..^_^");
    }else {
        QMessageBox::information(this, "error", "先搜索到结果后才能加哦..");
    }
}
void Profile::on_pushButtonAddGroupOk_clicked()
{

}
void Profile::newFriend(QString remark) {
    QTreeWidgetItem *friend_ = new QTreeWidgetItem(ui->treeWidget);
    friend_->setSizeHint(0, QSize(120, 40));
    friend_->setTextColor(0, Qt::red);
    friend_->setText(0, remark);
    friend_->setIcon(0, QIcon(":/images3/res/head.png"));
}
void Profile::friendDialog(QModelIndex index) {
    int b = index.row();
    emit friendDialogSignal(index.row());
}
void Profile::recvNewMsg(QString msg) {
    ui->textBrowserMsg->setTextColor(Qt::green);
    ui->textBrowserMsg->append(msg);
    ui->textBrowserMsg->moveCursor(QTextCursor::End);
}

void Profile::on_pushButtonModifySelectImage_clicked()
{
    QMessageBox::information(this, "呵呵", "功能正在开发中...");
}


void Profile::on_pushButtonEnterPublicGroup_clicked()
{
    emit showTalkWindow();
}

void Profile::on_pushButtonModifyPas_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(2);
}

void Profile::on_pushButtonCancelModifyInfo_clicked()
{

    ui->stackedWidget->setCurrentIndex(0);
    this->ui->lineEditModifyNickname->clear();
    this->ui->textEditModifySignature->clear();

}
void Profile::on_pushButtonCancelModifyPass_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(0);
    this->ui->lineEditModifyOrignalPassword->clear();
    this->ui->lineEditModifyPassword_1->clear();
    this->ui->lineEditModifyPassword_2->clear();
}

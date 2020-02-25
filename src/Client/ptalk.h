#ifndef PTALK_H
#define PTALK_H

#include <QWidget>
#include <QKeyEvent>
#include <QDebug>
#include <QPainter>
#include <QMessageBox>
#include <QDateTime>
namespace Ui {
class PTalk;
}

class PTalk : public QWidget
{
    Q_OBJECT

public:
    explicit PTalk(QWidget *parent, QString acc, QString nickname, QString gender, QString grade, QString signature, QString online);
    ~PTalk();
    void recvMsg(QString msg);
    void init();

private:
    Ui::PTalk *ui;
    QPoint mousePos;
    QString account;
    QString nickname;
    QString gender;

protected:
    bool eventFilter(QObject *, QEvent *);
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
signals:
    void sendMsgSginal(QString acc, QString msg);
    void hideWindow(QString acc);
private slots:
    void on_pushButtonSend_clicked();
    void on_pushButtonMinWindow_clicked();
    void on_pushButtonCloseWindow_clicked();
};


#endif // PTALK_H

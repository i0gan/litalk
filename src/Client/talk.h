#ifndef TALK_H
#define TALK_H

#include <QWidget>
#include <QDateTime>
#include <QKeyEvent>
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QFile>

namespace Ui {
class Talk;
}
class Talk : public QWidget
{
    Q_OBJECT

public:
    explicit Talk(QWidget *parent = 0);
    void init();
    void getMsg(QString nickname,QString msg);
    void newOne(QString nickname);
    void updatePersonList(QString personList);
    void loadCharRecord(); //加载对话记录
    ~Talk();

private slots:
    void on_pushButtonSend_clicked();
    void on_toolButtonClose_clicked();
    void on_toolButtonMax_clicked();
    void on_toolButtonMin_clicked();

signals:
    void sendMsg(QString msg);
private:
    Ui::Talk *ui;
    QPoint mousePos;
    bool isWindowMax;
    QSize windowSize;
    QFile *chatRecord;
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    bool eventFilter(QObject *, QEvent *);
    //void event(QEvent *);
};

#endif // TALK_H

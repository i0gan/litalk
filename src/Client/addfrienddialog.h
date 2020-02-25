#ifndef ADDFRIENDDIALOG_H
#define ADDFRIENDDIALOG_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
namespace Ui {
class AddfriendDialog;
}

class AddfriendDialog : public QWidget
{
    Q_OBJECT

public:
    explicit AddfriendDialog(QWidget *parent, QString account , QString pid , QString msg);
    ~AddfriendDialog();

private slots:
    void on_pushButtonAccept_clicked();
    void on_pushButtonReject_clicked();

    void on_pushButtonClose_clicked();

signals:
    void sendResult(int result, QString pid);
private:
    Ui::AddfriendDialog *ui;
    QString pid;
    QPoint mousePos;
protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
};

#endif // ADDFRIENDDIALOG_H

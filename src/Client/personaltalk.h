#ifndef PESONALTALK_H
#define PESONALTALK_H

#include <QWidget>

namespace Ui {
class PersonalTalk;
}

class PersonalTalk : public QWidget
{
    Q_OBJECT

public:
    explicit PersonalTalk(QWidget *parent = 0);
    ~PersonalTalk();

private:
    Ui::PersonalTalk *ui;
};

#endif // PESONALTALK_H

#include "personaltalk.h"
#include "ui_pesonaltalk.h"

PersonalTalk::PersonalTalk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonalTalk)
{
    ui->setupUi(this);
}

PersonalTalk::~PersonalTalk()
{
    delete ui;
}

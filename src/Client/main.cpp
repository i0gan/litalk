#include "control.h"
#include <QApplication>
#include <QTextCodec>
#include "profile.h"
int main(int argc, char *argv[])
{
    //解决中文乱码
//    QTextCodec *codec = QTextCodec::codecForName("utf-8");
//    QTextCodec::setCodecForLocale(codec);
//    QTextCodec::setCodecForCStrings(codec);
//    QTextCodec::setCodecForTr(codec);
    QApplication a(argc, argv);
    Control c;
    return a.exec();
}

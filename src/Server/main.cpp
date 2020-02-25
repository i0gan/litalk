#include <QCoreApplication>
#include <iostream>
#include <startserver.h>
#include <QDateTime>
//#define DEBUG
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << QString("\t\t%1\n\t\tLYXF Server").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm")).toUtf8().data();

#ifdef DEBUG
       StartServer *start = new StartServer(&a);
#else
    if(argc < 2) {
     std::cout << "Parameter:" << std::endl
               << "start            start the LYXF server" << std::endl
               << "stop             stop the LYXF server" << std::endl;
     exit(0);
     }
     QString parameter = argv[1];
     if(parameter == "start") {
       StartServer *start = new StartServer(&a);
     }else if(parameter == "stop") {

     }
#endif

    return a.exec();
}


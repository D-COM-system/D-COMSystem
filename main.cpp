#include "widget.h"
#include "ui_widget.h"
#include "user.cpp"
#include "record.cpp"
#include "dashboard.h"
#include "balance.cpp"
#include "entering.cpp"
#include "test.h"

#include <QApplication>
#include <QCoreApplication>
#include <QVBoxLayout>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    createUser(":/Data/user.xlsx",QCoreApplication::applicationDirPath());
    createRecord(":/Data/highquery.xlsx",QCoreApplication::applicationDirPath());
    createBalance(":/Data/balance.xlsx",QCoreApplication::applicationDirPath());
    createEntering(":/Data/RTGS.xlsx", QCoreApplication::applicationDirPath());
    Widget w;
    // 去掉窗体的默认标题栏
//    w.setWindowFlags(Qt::CustomizeWindowHint);
    w.show();
    dashboard w2;
    test w3;

//    QObject::connect(&w, SIGNAL(showdashboard()), &w2, SLOT(show()));
    QObject::connect(&w, SIGNAL(showdashboard()), &w3, SLOT(show()));
    // 将信号连接到槽函数

    return a.exec();
}

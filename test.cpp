#include "test.h"
#include "qscreen.h"
#include "ui_test.h"

test::test(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::test)
{
    ui->setupUi(this);
        // 获取默认屏幕
        QScreen* screen = QGuiApplication::primaryScreen();

        // 获取屏幕的几何矩形
        QRect screenGeometry = screen->geometry();

        // 获取屏幕的宽度和高度
        int screenWidth = screenGeometry.width();
        int screenHeight = screenGeometry.height();

        // 设置窗口大小
        int windowWidth = screenWidth * 0.8;  // 设置为屏幕宽度的80%
        int windowHeight = screenHeight * 0.8;  // 设置为屏幕高度的80%
        ui->widget_3->setFixedHeight(windowHeight);
        setFixedSize(windowWidth, windowHeight);  // 设置窗口固定大小
}

test::~test()
{
    delete ui;
}

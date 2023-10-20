#include "dashboard.h"
#include "ui_dashboard.h"

#include <QDateTime>
#include <QTimer>
#include <QMenu>
#include <QDebug>
#include <QContextMenuEvent>

dashboard::dashboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dashboard)
{
    ui->setupUi(this);
//    QFile file(":/Image/dashboard.qss");
//    file.open(QIODevice::ReadOnly);
//    setStyleSheet(file.readAll());
}



dashboard::~dashboard()
{
    delete ui;
}

void dashboard::recivelogin()
{
    this->show();
}

void dashboard::updateTime()
{
    // 获取当前时间
    QDateTime currentTime = QDateTime::currentDateTime();

    // 格式化时间为"YYYY-MM-DD HH:MM:SS"的字符串
    QString formattedTime = currentTime.toString("yyyy-MM-dd hh:mm:ss");

    // 设置lineEdit_2的文本为当前时间
    ui->lineEdit_2->setText(formattedTime);
}

void dashboard::initializeDashboard()
{
    ui->pushButton_5->setVisible(false);
    ui->pushButton_6->setVisible(false);
    ui->lineEdit_7->setText(globalUserName + "，欢迎使用D-COM业务终端！"); // 设置拼接后的文本到lineEdit_7
    ui->pushButton_3->setText(globalCusNumber);
    ui->tableWidget_2->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QDateTime currentTime = QDateTime::currentDateTime();
    QString formattedTime = currentTime.toString("yyyy-MM-dd hh:mm:ss");
    QTableWidgetItem *nowUser = new QTableWidgetItem(globalCusNumber);
    QTableWidgetItem *loginTime = new QTableWidgetItem(formattedTime);
    nowUser->setFlags(nowUser->flags() & (~Qt::ItemIsEditable));
    loginTime->setFlags(loginTime->flags() & (~Qt::ItemIsEditable));
    ui->tableWidget_2->setItem(0, 1, nowUser);
    ui->tableWidget_2->setItem(7, 1, loginTime);
    ui->tableWidget_2->item(0, 1)->setFont(QFont("Microsoft YaHei UI", 12));
    ui->tableWidget_2->item(7, 1)->setFont(QFont("Microsoft YaHei UI", 12));
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 2; j++) {
            QTableWidgetItem* item = ui->tableWidget_2->item(i, j);
            if (item) {
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
        }
    }

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &dashboard::updateTime);
    timer->start(1000);

    QWidget* widget_4 = findChild<QWidget*>("widget_4");

    widget_4->setVisible(false);
    QToolButton* toolButton_4 = findChild<QToolButton*>("toolButton_4");
    QToolButton* toolButton = findChild<QToolButton*>("toolButton");
    if (toolButton_4) {
        // 监听鼠标进入事件
        toolButton_4->installEventFilter(this);
        widget_4->installEventFilter(this);
    }
    if (toolButton) {
        // 监听鼠标进入事件
        toolButton->installEventFilter(this);
        widget_4->installEventFilter(this);
    }
    connect(ui->pushButton_5, &QPushButton::clicked, this, &dashboard::reDashboard);
    connect(ui->pushButton, &QPushButton::clicked, this, &dashboard::reDashboard);
}

void dashboard::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    initializeDashboard();
}

bool dashboard::eventFilter(QObject* object, QEvent* event) {
    QWidget* widget_4 = findChild<QWidget*>("widget_4");
    QToolButton* toolButton_4 = findChild<QToolButton*>("toolButton_4");
    QToolButton* toolButton = findChild<QToolButton*>("toolButton");
    if (object == toolButton_4) {
        if (event->type() == QEvent::Enter) {
            QTreeWidget* treeWidget = new QTreeWidget(widget_4);
            if (!treeWidget) {
                treeWidget = new QTreeWidget(widget_4);
                treeWidget->setObjectName("treeWidget");
                widget_4->layout()->addWidget(treeWidget);
            }
            createFundMenu(treeWidget);
            // 设置 QTreeWidget 的大小与 widget_4 相同
            treeWidget->setGeometry(widget_4->rect());

            // 设置左间隔为 35px
            QString styleSheet = "QTreeWidget { "
                                 "padding-left: 35px;"
                                 "padding-top:30px;"
                                 " }";
            treeWidget->setStyleSheet(styleSheet);

            // 鼠标进入 toolButton_4，将 widget_4 显示
            widget_4->setVisible(true);
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开 toolButton_4，检查鼠标是否进入 widget_4
            if (!widget_4->underMouse()) {
                // 鼠标没有进入 widget_4，将 widget_4 隐藏
                widget_4->setVisible(false);
            }
        }
    }
    else if(object == toolButton) {
        if (event->type() == QEvent::Enter) {
            QTreeWidget* treeWidget = new QTreeWidget(widget_4);
            if (!treeWidget) {
                treeWidget = new QTreeWidget(widget_4);
                treeWidget->setObjectName("treeWidget");
                widget_4->layout()->addWidget(treeWidget);
            }
            createRTGSMenu(treeWidget);
            // 设置 QTreeWidget 的大小与 widget_4 相同
            treeWidget->setGeometry(widget_4->rect());

            // 设置左间隔为 35px
            QString styleSheet = "QTreeWidget { "
                                 "padding-left: 35px;"
                                 "padding-top:30px;"
                                 " }";
            treeWidget->setStyleSheet(styleSheet);

            // 鼠标进入 toolButton_4，将 widget_4 显示
            widget_4->setVisible(true);
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开 toolButton_4，检查鼠标是否进入 widget_4
            if (!widget_4->underMouse()) {
                // 鼠标没有进入 widget_4，将 widget_4 隐藏
                widget_4->setVisible(false);
            }
        }
    }
    else if (object == widget_4) {
        if (event->type() == QEvent::Enter) {
            // 鼠标进入 widget_4，将 widget_4 显示
            widget_4->setVisible(true);
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开 widget_4，将 widget_4 隐藏
            widget_4->setVisible(false);
        }
    }
    // 继续传递事件
    return QObject::eventFilter(object, event);
}

void dashboard::createFundMenu(QTreeWidget* treeWidget) {
    treeWidget->clear();
    // 取消垂直滚动条
    treeWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 取消水平滚动条
    treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 创建导航栏
    treeWidget->header()->hide();

    // 创建根节点
    QTreeWidgetItem* rootItem = new QTreeWidgetItem(treeWidget);
    rootItem->setText(0, "资金划拨");

    // 在两个根节点之间插入一个空白的占位节点
    QTreeWidgetItem* spacerItem = new QTreeWidgetItem(treeWidget);
    spacerItem->setSizeHint(0, QSize(0, 50)); // 设置占位节点的高度为 50 像素

    QTreeWidgetItem* rootItem2 = new QTreeWidgetItem(treeWidget);
    rootItem2->setText(0, "信息查询");

    // 创建子节点并添加到根节点
    QTreeWidgetItem* item1 = new QTreeWidgetItem(rootItem);
    item1->setText(0, "资金余额查询");

    QTreeWidgetItem* item2 = new QTreeWidgetItem(rootItem);
    item2->setText(0, "凭证录入");

    QTreeWidgetItem* item3 = new QTreeWidgetItem(rootItem);
    item3->setText(0, "凭证复核");

    QTreeWidgetItem* item4 = new QTreeWidgetItem(rootItem);
    item4->setText(0, "凭证授权");

    QTreeWidgetItem* item5 = new QTreeWidgetItem(rootItem);
    item5->setText(0, "凭证导入");

    QTreeWidgetItem* item6 = new QTreeWidgetItem(rootItem);
    item6->setText(0, "凭证发送");

    QTreeWidgetItem* item7 = new QTreeWidgetItem(rootItem2);
    item7->setText(0, "现货资金账户余额批量查询");

    QTreeWidgetItem* item8 = new QTreeWidgetItem(rootItem2);
    item8->setText(0, "银行进出账查询设置");

    QTreeWidgetItem* item9 = new QTreeWidgetItem(rootItem2);
    item9->setText(0, "结算备付金账户明细查询");

    QTreeWidgetItem* item10 = new QTreeWidgetItem(rootItem2);
    item10->setText(0, "结算保证金账户明细查询");

    QTreeWidgetItem* item11 = new QTreeWidgetItem(rootItem2);
    item11->setText(0, "结息凭证数据查询");

    QTreeWidgetItem* item12 = new QTreeWidgetItem(rootItem2);
    item12->setText(0, "结算备付金账户历史余额查询");

    // 设置树形控件的整体缩进值
    treeWidget->setIndentation(10);

    // 设置根节点的字体
    QFont rootFont;
    rootFont.setPointSize(10);  // 设置字体大小
    rootItem->setFont(0, rootFont);
    rootItem2->setFont(0, rootFont);

    // 设置根节点的颜色
    rootItem->setForeground(0, QBrush(QColor(Qt::white)));
    rootItem2->setForeground(0, QBrush(QColor(Qt::white)));

    // 设置子节点的字体
    QFont childFont;
    childFont.setPointSize(10);  // 设置字体大小
    item1->setFont(0, childFont);
    item2->setFont(0, childFont);
    item3->setFont(0, childFont);
    item4->setFont(0, childFont);
    item5->setFont(0, childFont);
    item6->setFont(0, childFont);
    item7->setFont(0, childFont);
    item8->setFont(0, childFont);
    item9->setFont(0, childFont);
    item10->setFont(0, childFont);
    item11->setFont(0, childFont);
    item12->setFont(0, childFont);
    // 设置子节点的颜色
    item1->setForeground(0, QBrush(QColor(Qt::white)));
    item2->setForeground(0, QBrush(QColor(Qt::white)));
    item3->setForeground(0, QBrush(QColor(Qt::white)));
    item4->setForeground(0, QBrush(QColor(Qt::white)));
    item5->setForeground(0, QBrush(QColor(Qt::white)));
    item6->setForeground(0, QBrush(QColor(Qt::white)));
    item7->setForeground(0, QBrush(QColor(Qt::white)));
    item8->setForeground(0, QBrush(QColor(Qt::white)));
    item9->setForeground(0, QBrush(QColor(Qt::white)));
    item10->setForeground(0, QBrush(QColor(Qt::white)));
    item11->setForeground(0, QBrush(QColor(Qt::white)));
    item12->setForeground(0, QBrush(QColor(Qt::white)));

    // 连接 itemClicked 信号和槽函数
    connect(treeWidget, &QTreeWidget::itemClicked, this, &dashboard::onTreeWidgetItemClicked);
}

void dashboard::createRTGSMenu(QTreeWidget* treeWidget) {
    treeWidget->clear();
    // 取消垂直滚动条
    treeWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 取消水平滚动条
    treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 创建导航栏
    treeWidget->header()->hide();
    // 创建根节点
    QTreeWidgetItem* rootItem = new QTreeWidgetItem(treeWidget);
    rootItem->setText(0, "B股指令");
    // 在两个根节点之间插入一个空白的占位节点
    QTreeWidgetItem* spacerItem1 = new QTreeWidgetItem(treeWidget);
    spacerItem1->setSizeHint(0, QSize(0, 50)); // 设置占位节点的高度为 50 像素
    QTreeWidgetItem* rootItem2 = new QTreeWidgetItem(treeWidget);
    rootItem2->setText(0, "RTGS交收");
    // 在两个根节点之间插入一个空白的占位节点
    QTreeWidgetItem* spacerItem2 = new QTreeWidgetItem(treeWidget);
    spacerItem2->setSizeHint(0, QSize(0, 50)); // 设置占位节点的高度为 50 像素
    QTreeWidgetItem* rootItem3 = new QTreeWidgetItem(treeWidget);
    rootItem3->setText(0, "证券交易资金前端控制");

    // 创建子节点并添加到根节点
    QTreeWidgetItem* item1 = new QTreeWidgetItem(rootItem);
    item1->setText(0, "一类指令修改");

    QTreeWidgetItem* item2 = new QTreeWidgetItem(rootItem);
    item2->setText(0, "二类指令修改");

    QTreeWidgetItem* item3 = new QTreeWidgetItem(rootItem2);
    item3->setText(0, "RTGS录入(勾选)");

    QTreeWidgetItem* item4 = new QTreeWidgetItem(rootItem2);
    item4->setText(0, "RTGS指令申报");

    QTreeWidgetItem* item5 = new QTreeWidgetItem(rootItem2);
    item5->setText(0, "RTGS查看回报(参与人入口)");

    QTreeWidgetItem* item6 = new QTreeWidgetItem(rootItem2);
    item6->setText(0, "设置");

    QTreeWidgetItem* item7 = new QTreeWidgetItem(rootItem3);
    item7->setText(0, "最高额度常规申报");

    QTreeWidgetItem* item8 = new QTreeWidgetItem(rootItem3);
    item8->setText(0, "最高额度常规申报查询");

    QTreeWidgetItem* item9 = new QTreeWidgetItem(rootItem3);
    item9->setText(0, "最高额度盘中紧急申报");

    QTreeWidgetItem* item10 = new QTreeWidgetItem(rootItem3);
    item10->setText(0, "最高额度盘中紧急申报查询");

    QTreeWidgetItem* item11 = new QTreeWidgetItem(rootItem3);
    item11->setText(0, "最高额度盘中紧急申报撤销");

    // 设置树形控件的整体缩进值
    treeWidget->setIndentation(10);

    // 设置根节点的字体
    QFont rootFont;
    rootFont.setPointSize(10);  // 设置字体大小
    rootItem->setFont(0, rootFont);
    rootItem2->setFont(0, rootFont);
    rootItem3->setFont(0, rootFont);

    // 设置根节点的颜色
    rootItem->setForeground(0, QBrush(QColor(Qt::white)));
    rootItem2->setForeground(0, QBrush(QColor(Qt::white)));
    rootItem3->setForeground(0, QBrush(QColor(Qt::white)));

    // 设置子节点的字体
    QFont childFont;
    childFont.setPointSize(10);  // 设置字体大小
    item1->setFont(0, childFont);
    item2->setFont(0, childFont);
    item3->setFont(0, childFont);
    item4->setFont(0, childFont);
    item5->setFont(0, childFont);
    item6->setFont(0, childFont);
    item7->setFont(0, childFont);
    item8->setFont(0, childFont);
    item9->setFont(0, childFont);
    item10->setFont(0, childFont);
    item11->setFont(0, childFont);
    // 设置子节点的颜色
    item1->setForeground(0, QBrush(QColor(Qt::white)));
    item2->setForeground(0, QBrush(QColor(Qt::white)));
    item3->setForeground(0, QBrush(QColor(Qt::white)));
    item4->setForeground(0, QBrush(QColor(Qt::white)));
    item5->setForeground(0, QBrush(QColor(Qt::white)));
    item6->setForeground(0, QBrush(QColor(Qt::white)));
    item7->setForeground(0, QBrush(QColor(Qt::white)));
    item8->setForeground(0, QBrush(QColor(Qt::white)));
    item9->setForeground(0, QBrush(QColor(Qt::white)));
    item10->setForeground(0, QBrush(QColor(Qt::white)));
    item11->setForeground(0, QBrush(QColor(Qt::white)));

    // 连接 itemClicked 信号和槽函数
    connect(treeWidget, &QTreeWidget::itemClicked, this, &dashboard::onTreeWidgetItemClicked);
}

void dashboard::onTreeWidgetItemClicked(QTreeWidgetItem* item, int column) {
    // 处理点击事件
    QTreeWidgetItem *parentItem = item->parent(); // 获取父节点
    QString rootNodeText = parentItem ? parentItem->text(column) : "";
    QString text = item->text(column);
//    QWidget* dashboardwidget_5 = findChild<QWidget*>("widget_5");
    ui->pushButton_5->setVisible(true);
    ui->pushButton_6->setVisible(true);
    if(text == "资金余额查询") {
        ui->pushButton_6->setText("/" + rootNodeText + "/" + text);
        fundbalancequery = new FundBalanceQuery(this);
        QWidget *fundbalancequerywidget_4 = fundbalancequery->ui->widget_4;
        ui->stackedWidget->addWidget(fundbalancequerywidget_4);
        ui->stackedWidget->setCurrentWidget(fundbalancequerywidget_4);
    }
    else if(text == "RTGS录入(勾选)")
    {
        ui->pushButton_6->setText("/" + rootNodeText + "/" + text);
        RTGSentryquery = new RTGSEntryQuery(this);
        QWidget *RTGSentryquerywidget_4 = RTGSentryquery->ui->widget_4;
        ui->stackedWidget->addWidget(RTGSentryquerywidget_4);
        ui->stackedWidget->setCurrentWidget(RTGSentryquerywidget_4);
    }
    else if(text == "最高额度常规申报")
    {
        ui->pushButton_6->setText("/" + rootNodeText + "/" + text);
        maximumamountregulardeclaration = new MaximumAmountRegularDeclaration(this);
        QWidget *maximumamountregulardeclarationwidget_4 = maximumamountregulardeclaration->ui->widget_4;
        ui->stackedWidget->addWidget(maximumamountregulardeclarationwidget_4);
        ui->stackedWidget->setCurrentWidget(maximumamountregulardeclarationwidget_4);
        connect(maximumamountregulardeclaration, &MaximumAmountRegularDeclaration::informationReturned, this, &dashboard::handleReturnedInformation);
    }
}

void dashboard::handleReturnedInformation()
{
    importofdeclarations = new ImportOfDeclarations(this);
    QWidget *importofdeclarationswidget_4 = importofdeclarations->ui->widget_4;
    ui->stackedWidget->addWidget(importofdeclarationswidget_4);
    ui->stackedWidget->setCurrentWidget(importofdeclarationswidget_4);
    connect(importofdeclarations, &ImportOfDeclarations::informationBack, this, &dashboard::handleBackInformation);
}

void dashboard::handleBackInformation() {
    maximumamountregulardeclaration = new MaximumAmountRegularDeclaration(this);
    QWidget *maximumamountregulardeclarationwidget_4 = maximumamountregulardeclaration->ui->widget_4;
    ui->stackedWidget->addWidget(maximumamountregulardeclarationwidget_4);
    ui->stackedWidget->setCurrentWidget(maximumamountregulardeclarationwidget_4);
    connect(maximumamountregulardeclaration, &MaximumAmountRegularDeclaration::informationReturned, this, &dashboard::handleReturnedInformation);
}

void dashboard::reDashboard() {
    ui->pushButton_5->setVisible(false);
    ui->pushButton_6->setVisible(false);
    QWidget* dashboardwidget_5 = findChild<QWidget*>("widget_5");
    ui->stackedWidget->addWidget(dashboardwidget_5);
    ui->stackedWidget->setCurrentWidget(dashboardwidget_5);
}
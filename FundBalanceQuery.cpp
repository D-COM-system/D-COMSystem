#include "FundBalanceQuery.h"
#include "ui_FundBalanceQuery.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

FundBalanceQuery::FundBalanceQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FundBalanceQuery)
{
    ui->setupUi(this);
    comboBoxAddItem();
    // 设置下拉框的颜色
    QString comboBoxStyle = "QComboBox {"
                            "background-color: #FFFFFF;"  // 设置背景色
                            "color: #000000;"  // 设置文字颜色
                            "selection-background-color: #C0C0C0;"  // 设置选中项的背景色
                            "selection-color: #000000;"  // 设置选中项的文字颜色
                            "}";

    // 设置子项之间的间距
    comboBoxStyle += "QComboBox QAbstractItemView {"
                     "spacing: 10px;"  // 设置子项之间的间距
                     "}";

    ui->comboBox->setStyleSheet(comboBoxStyle);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &FundBalanceQuery::siftToData);
}

FundBalanceQuery::~FundBalanceQuery()
{
    delete ui;
}

void FundBalanceQuery::comboBoxAddItem() {
    ui->comboBox->setEditable(false);
    // 连接数据库
    QString dbName = "database.db";
    QString dbPath = QCoreApplication::applicationDirPath() + "./" + dbName;  // Use a relative path
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dbPath);

    if (!database.open()) {
        QMessageBox::critical(nullptr, "错误", "无法打开数据库：" + database.lastError().text());
        return;
    }

    // 执行查询
    QSqlQuery query;
    query.prepare("SELECT account FROM balance");
    if (query.exec()) {
        ui->comboBox->addItem("请选择");
        while (query.next()) {
            QString account = query.value(0).toString();
            ui->comboBox->addItem(account);
        }
        ui->comboBox->setStyleSheet("QComboBox QAbstractItemView { background-color: yellow; color: blue; }");
    } else {
        QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
        return;
    }

    // 关闭数据库连接
    database.close();
}

void FundBalanceQuery::siftToData() {
    ui->comboBox_2->setEditable(true);
    QString account = ui->comboBox->currentText().trimmed();  // 移除前导和尾部的空格
    qDebug() << account;
    QString dbName = "database.db";
    QString dbPath = QCoreApplication::applicationDirPath() + "./" + dbName;  // Use a relative path
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dbPath);

    if (!database.open()) {
        QMessageBox::critical(nullptr, "错误", "无法打开数据库：" + database.lastError().text());
        return;
    }

    // 执行查询
    QSqlQuery query;
    query.prepare("SELECT * FROM balance WHERE account = :account");
    query.bindValue(":account", account);
    if (query.exec()) {
        if (query.next()) {
            //        QString guidResult = query.value("guid").toString();
            //        QString accountResult = query.value("account").toString();
            QString nameResult = query.value("name").toString();
            QString currencyResult = query.value("currency").toString();
            QString presentResult = QString("%1").arg(query.value("present").toDouble(), 0, 'f', 2);
            QString startResult = QString("%1").arg(query.value("start").toDouble(), 0, 'f', 2);
            QString productResult = QString("%1").arg(query.value("product").toDouble(), 0, 'f', 2);
            QString nonpaymentResult = QString("%1").arg(query.value("nonpayment").toDouble(), 0, 'f', 2);
            QString drawingResult = QString("%1").arg(query.value("drawing").toDouble(), 0, 'f', 2);
            QString daytimeResult = QString("%1").arg(query.value("daytime").toDouble(), 0, 'f', 2);
            QString a_depositResult = QString("%1").arg(query.value("a_deposit").toDouble(), 0, 'f', 2);
            QString day_saveResult = QString("%1").arg(query.value("day_save").toDouble(), 0, 'f', 2);
            QString a_withdrawalResult = QString("%1").arg(query.value("a_withdrawal").toDouble(), 0, 'f', 2);
            QString day_withdrawResult = QString("%1").arg(query.value("day_withdraw").toDouble(), 0, 'f', 2);
            QString minResult = QString("%1").arg(query.value("min").toDouble(), 0, 'f', 2);
            QString stateResult = query.value("state").toString();
            ui->lineEdit_7->setText(nameResult);
            ui->comboBox_2->setEditText(currencyResult);
            ui->lineEdit_8->setText(presentResult);
            ui->lineEdit_9->setText(productResult);
            ui->lineEdit_10->setText(drawingResult);
            ui->lineEdit_11->setText(a_depositResult);
            ui->lineEdit_12->setText(a_withdrawalResult);
            ui->lineEdit_13->setText(minResult);
            ui->lineEdit_14->setText(startResult);
            ui->lineEdit_15->setText(nonpaymentResult);
            ui->lineEdit_16->setText(daytimeResult);
            ui->lineEdit_17->setText(day_saveResult);
            ui->lineEdit_18->setText(day_withdrawResult);
            ui->lineEdit_19->setText(stateResult);
        } else {
             QMessageBox::critical(nullptr, "错误", "查询执行失败：无对应数据");
        }

    } else {
        QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
        return;
    }

    // 关闭数据库连接
    database.close();
}

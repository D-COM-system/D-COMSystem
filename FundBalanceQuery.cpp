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
    connect(ui->pushButton_5, &QPushButton::clicked, this, &FundBalanceQuery::siftToData);
}

FundBalanceQuery::~FundBalanceQuery()
{
    delete ui;
}

void FundBalanceQuery::comboBoxAddItem() {
    ui->comboBox->setEditable(true);
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
        while (query.next()) {
            QString account = query.value(0).toString();
            ui->comboBox->addItem(account);
        }
        ui->comboBox->setEditText("请选择");
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
            QString presentResult = query.value("present").toString();
            QString startResult = query.value("start").toString();
            QString productResult = query.value("product").toString();
            QString nonpaymentResult = query.value("nonpayment").toString();
            QString drawingResult = query.value("drawing").toString();
            QString daytimeResult = query.value("daytime").toString();
            QString a_depositResult = query.value("a_deposit").toString();
            QString day_saveResult = query.value("day_save").toString();
            QString a_withdrawalResult = query.value("a_withdrawal").toString();
            QString day_withdrawResult = query.value("day_withdraw").toString();
            QString minResult = query.value("min").toString();
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

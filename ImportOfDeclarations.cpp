#include "ImportOfDeclarations.h"
#include "ui_ImportOfDeclarations.h"
#include "xlsxdocument.h"
#include "xlsxworksheet.h"

#include <QFile>
#include <QTextStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDateTime>


QString nowFilePath = "";

ImportOfDeclarations::ImportOfDeclarations(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImportOfDeclarations)
{
    ui->setupUi(this);
    ui->widget_5->setVisible(false);
    connect(ui->pushButton_19, &QPushButton::clicked, this, &ImportOfDeclarations::backToImport);
    connect(ui->pushButton_5, &QPushButton::clicked, this,&ImportOfDeclarations::chooseFile);
    connect(ui->pushButton_17, &QPushButton::clicked, this,&ImportOfDeclarations::importFile);
    connect(ui->pushButton_18, &QPushButton::clicked, this, &ImportOfDeclarations::resetText);
    connect(ui->pushButton_6, &QPushButton::clicked, this,&ImportOfDeclarations::closeMessageWindow);
    connect(ui->toolButton_11, &QToolButton::clicked, this, &ImportOfDeclarations::closeMessageWindow);
}

ImportOfDeclarations::~ImportOfDeclarations()
{
    delete ui;
}

void ImportOfDeclarations::backToImport()
{
    emit informationBack();
}

void  ImportOfDeclarations::chooseFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select File", "", "CSV Files (*.csv);;Excel Files (*.xlsx)");
    if (filePath.isEmpty()) {
        qDebug() << "Selected File: isEmpty";
        // 这里可以处理所选文件的路径
    }
    QString nativePath = QDir::toNativeSeparators(filePath);

    // 将路径分割为各个部分
    QStringList pathParts = nativePath.split(QDir::separator(), QString::SkipEmptyParts);

    // 移除盘符（如Macintosh HD）
    if (pathParts.size() > 0) {
        pathParts.removeFirst();
    }

    // 按照指定格式拼接路径
    QString convertedPath = pathParts.join(" ▶ ");

    // 将路径分隔符转换回原生格式
    convertedPath = QDir::fromNativeSeparators(convertedPath);

    ui->lineEdit_6->setText(convertedPath);
    nowFilePath = filePath;
}

void ImportOfDeclarations::importFile()
{
    if(ui->lineEdit_6->text().isEmpty()) {
        return ;
    }
    int successInsert = 0;
    QString dbName = "database.db";
    QString dbPath = QCoreApplication::applicationDirPath() + "./" + dbName;  // Use a relative path

    // Create a database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        QMessageBox::critical(nullptr, "错误", "数据库打开失败：" + db.lastError().text());
        return;
    }

    // Load the Excel file
    QXlsx::Document excelFile(nowFilePath);
    // Assuming the data is in the first sheet
    QXlsx::Worksheet* sheet = excelFile.currentWorksheet();

    QSqlQuery query;
    // Assuming the data starts from row 2 (skip header row)
    for (int row = 2; row <= sheet->dimension().lastRow(); ++row) {
        QString guid = sheet->read(row, 1).toString();
        QString state = sheet->read(row, 2).toString();
        QString institutionCode = sheet->read(row, 3).toString();
        QString institutionName = sheet->read(row, 4).toString();
        QString TotalAssets = sheet->read(row, 5).toString();
        QString MaxAccount = sheet->read(row, 6).toString();
        QString ControlClass = sheet->read(row, 7).toString();
        QString dataEntryClerk = sheet->read(row, 8).toString();
        // 获取当前时间
        QDateTime currentTime = QDateTime::currentDateTime();
        // 将当前时间转换为字符串
        QString inputTime = currentTime.toString("yyyy-MM-dd hh:mm:ss");

        // 检查所有元素是否都为空
        if (guid.isEmpty() && state.isEmpty() && institutionCode.isEmpty() && institutionName.isEmpty() &&
            TotalAssets.isEmpty() && MaxAccount.isEmpty() && ControlClass.isEmpty() && dataEntryClerk.isEmpty()) {
            continue;  // 跳过当前行，不执行插入操作
        }

        QString insertQuery = "INSERT INTO record (guid, state, institutionCode, institutionName, TotalAssets, MaxAccount, ControlClass, dataEntryClerk, inputTime) "
                              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
        query.prepare(insertQuery);
        query.addBindValue(guid);
        query.addBindValue(state);
        query.addBindValue(institutionCode);
        query.addBindValue(institutionName);
        query.addBindValue(TotalAssets);
        query.addBindValue(MaxAccount);
        query.addBindValue(ControlClass);
        query.addBindValue(dataEntryClerk);
        query.addBindValue(inputTime);

            if (!query.exec()) {
                qDebug() << "插入数据失败：" << query.lastError().text();
            } else {
                successInsert++;
            }
    }
    // Close the database connection
    db.close();
//    qDebug() << successInsert;
    ui->lineEdit_6->clear();
    ui->lineEdit_9->setText("本次共导入" + QString::number(successInsert) + "条数据");
    ui->widget_5->setVisible(true);
}

void ImportOfDeclarations::resetText() {
    ui->lineEdit_6->clear();
}

void ImportOfDeclarations::closeMessageWindow() {
    ui->widget_5->setVisible(false);
}

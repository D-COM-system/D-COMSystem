#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include "xlsxdocument.h"

void createRecord(QString excelFilePath, QString dirPath)
{
    QString dbName = "database.db";
    QString dbPath = dirPath + "./" + dbName;  // Use a relative path

    // Create a database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        QMessageBox::critical(nullptr, "错误", "数据库打开失败：" + db.lastError().text());
        return;
    }

    // Load the Excel file
    QXlsx::Document excelFile(excelFilePath);

    // Assuming the data is in the first sheet
    QXlsx::Worksheet* sheet = excelFile.currentWorksheet();

    QSqlQuery query;
    QString createQuery = "CREATE TABLE record (guid varchar(64) primary key,state varchar(10), institutionCode  varchar(20), institutionName  varchar(50),TotalAssets int(13), MaxAccount int(13), ControlClass varchar(10), dataEntryClerk varchar(10), inputTime DATETIME)";
    if (!query.exec(createQuery)) {
        qDebug() << "创建表失败：" << query.lastError().text();
        return;
    }

    bool createTableSuccess = true;
    for (int row = 2; row <= sheet->dimension().rowCount(); ++row) {
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
        // 格式化时间为"YYYY-MM-DD HH:MM:SS"的字符串
        QString formattedTime = currentTime.toString("yyyy-MM-dd hh:mm:ss");
        // 设置当前时间
        QString inputTime = formattedTime;

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

        if (createTableSuccess && !query.exec()) {
            qDebug() << "插入数据失败：" << query.lastError().text();
                                                       createTableSuccess = false;
        }
        if (!createTableSuccess) {
            // 在插入数据失败后的处理逻辑
            // 可以选择终止循环或进行其他处理
            break;
        }
    }
    // Close the database connection
    db.close();
}

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "xlsxdocument.h"

void createEntering(QString excelFilePath, QString dirPath)
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
    QString createQuery = "CREATE TABLE entering (guid varchar(20),state varchar(10),serial_number varchar(50) primary key ,account varchar(20),number varchar(50),provision_number varchar(10),type varchar(20),code varchar(20),securities_account varchar(20))";
    if (!query.exec(createQuery)) {
        qDebug() << "创建表失败：" << query.lastError().text();
        return;
    }

    bool createTableSuccess = true;
    for (int row = 2; row <= sheet->dimension().rowCount(); ++row) {
        QString guid = sheet->read(row, 1).toString();
        QString state = sheet->read(row, 2).toString();
        QString serial_number = sheet->read(row, 3).toString();
        QString account = sheet->read(row, 4).toString();
        QString number = sheet->read(row, 5).toString();
        QString provision_number = sheet->read(row, 6).toString();
        QString type = sheet->read(row, 7).toString();
        QString code = sheet->read(row, 8).toString();
        QString securities_account = sheet->read(row, 9).toString();

        // 检查所有元素是否都为空
        if (guid.isEmpty() && state.isEmpty() && serial_number.isEmpty() && account.isEmpty() &&
            number.isEmpty() && provision_number.isEmpty() && type.isEmpty() && code.isEmpty() &&
            securities_account.isEmpty()) {
            continue;  // 跳过当前行，不执行插入操作
        }

        QString insertQuery = "INSERT INTO entering (guid,state,serial_number,account,number,provision_number,type,code,securities_account) "
                              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
        query.prepare(insertQuery);
        query.addBindValue(guid);
        query.addBindValue(state);
        query.addBindValue(serial_number);
        query.addBindValue(account);
        query.addBindValue(number);
        query.addBindValue(provision_number);
        query.addBindValue(type);
        query.addBindValue(code);
        query.addBindValue(securities_account);

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



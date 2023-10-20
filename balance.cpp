#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "xlsxdocument.h"
#include "QtXlsx/QtXlsx"

void createBalance(QString excelFilePath, QString dirPath)
{
    QString dbName = "database.db";
    QString dbPath = dirPath + "/" + dbName;  // 使用相对路径

    // 创建数据库连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        QMessageBox::critical(nullptr, "错误", "无法打开数据库：" + db.lastError().text());
        return;
    }

    // 加载Excel文件
    QXlsx::Document excelFile(excelFilePath);

    QXlsx::Worksheet* sheet = excelFile.currentWorksheet();

    QSqlQuery query;
    QString createQuery = "CREATE TABLE balance (guid varchar(64),account varchar(30) primary key ,name varchar(64),currency varchar(10),present decimal(17),start decimal(17),product decimal(17),nonpayment decimal(17),drawing decimal(17),daytime dicimal(17),a_deposit int,day_save decimal(17),a_withdrawal int,day_withdraw decimal(17),min decimal(17),state varchar(10))";
    if (!query.exec(createQuery)) {
        qDebug() << "创建表失败：" << query.lastError().text();
        return;
    }

    bool createTableSuccess = true;
    for (int row = 2; row <= sheet->dimension().rowCount(); ++row) {
        QString guid = sheet->read(row, 1).toString();
        QString account = sheet->read(row, 2).toString();
        QString name = sheet->read(row, 3).toString();
        QString currency = sheet->read(row, 4).toString();
        QString present = sheet->read(row, 5).toString();
        QString start = sheet->read(row, 6).toString();
        QString product = sheet->read(row, 7).toString();
        QString nopayment = sheet->read(row, 8).toString();
        QString drawing = sheet->read(row, 9).toString();
        QString daytime = sheet->read(row, 10).toString();
        QString a_deposit = sheet->read(row, 11).toString();
        QString day_save = sheet->read(row, 12).toString();
        QString a_withdrawal = sheet->read(row, 13).toString();
        QString day_withdraw = sheet->read(row, 14).toString();
        QString min = sheet->read(row, 15).toString();
        QString state = sheet->read(row, 16).toString();

        // 检查所有元素是否都为空
        if (guid.isEmpty() && account.isEmpty() && name.isEmpty() && currency.isEmpty() &&
            present.isEmpty() && start.isEmpty() && product.isEmpty() && nopayment.isEmpty() &&
            drawing.isEmpty() && daytime.isEmpty() && a_deposit.isEmpty() && day_save.isEmpty() &&
            a_withdrawal.isEmpty() && day_withdraw.isEmpty() && min.isEmpty() && state.isEmpty()) {
            continue;  // 跳过当前行，不执行插入操作
        }

        QString insertQuery = "INSERT INTO balance (guid,account,name,currency,present,start,product,nonpayment,drawing,daytime,a_deposit,day_save,a_withdrawal,day_withdraw,min,state) "
                              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,?,?,?)";
        query.prepare(insertQuery);
        query.addBindValue(guid);
        query.addBindValue(account);
        query.addBindValue(name);
        query.addBindValue(currency);
        query.addBindValue(present);
        query.addBindValue(start);
        query.addBindValue(product);
        query.addBindValue(nopayment);
        query.addBindValue(drawing);
        query.addBindValue(daytime);
        query.addBindValue(a_deposit);
        query.addBindValue(day_save);
        query.addBindValue(a_withdrawal);
        query.addBindValue(day_withdraw);
        query.addBindValue(min);
        query.addBindValue(state);

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

    // 关闭数据库连接
    db.close();
}

 #include "RTGSEntryQuery.h"
#include "ui_RTGSEntryQuery.h"

RTGSEntryQuery::RTGSEntryQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RTGSEntryQuery)
{
    ui->setupUi(this);
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
    query.prepare("SELECT COUNT(*) FROM entering");
    if (query.exec()) {
        if (query.next()) {
            totalRows = query.value(0).toInt();
        }
        // 删除空行
    } else {
        QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
        return;
    }
    // 关闭数据库连接
    database.close();
    currentPage = 0,
    pageSize = 50,
    totalPages = ((totalRows + pageSize - 1) / pageSize);
    QStringList headerLabels;
    headerLabels << "勾选" << "划付序号" << "处理状态" << "清算业务流水号" << "业务流水号"
                 << "结算账户" << "执行编号" << "备付金账户" << "清算业务类别"
                 << "证券代码" << "证券账户";
    ui->tableWidget_2->clear(); // 清空表格内容
    ui->tableWidget_2->setColumnCount(headerLabels.size());
    ui->tableWidget_2->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->setRowCount(pageSize);
    //让tableWidget内容中的每个元素居中
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);//设置整行选中
    ui->tableWidget_2->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);//表头字体居中
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑
    // 设置表头的底色和文字加粗
    QString styleSheet = "QHeaderView::section { background-color: #f5f5f5; font-weight: bold; }";
    // 将样式表应用于表格的表头
    ui->tableWidget_2->horizontalHeader()->setStyleSheet(styleSheet);

    updateTableDisplay();
//    qDebug() << totalRows;
    boolArray.resize(totalRows);
    for (int i = 0; i < totalRows; ++i)
    {
        boolArray[i] = false; // 默认赋值为 false
    }
    // 隐藏左侧的行号框
    ui->tableWidget_2->verticalHeader()->setVisible(false);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &RTGSEntryQuery::previousPageButton_clicked);
    connect(ui->pushButton_10, &QPushButton::clicked, this, &RTGSEntryQuery::nextPageButton_clicked);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &RTGSEntryQuery::siftToData);
}

RTGSEntryQuery::~RTGSEntryQuery()
{
    delete ui;
}

void RTGSEntryQuery::selectRows()
{
    for (int var = 0; var < pageSize; var++) {
        int row = var + (currentPage * pageSize); // 计算当前页面的实际行索引
        if (row < totalRows) {
            QCheckBox* checkBox = qobject_cast<QCheckBox*>(ui->tableWidget_2->cellWidget(var, 0));
            if (checkBox) {
                int rowIndex = var + (currentPage * pageSize);
                boolArray[row] = checkBox->isChecked();
                checkboxStateMap[rowIndex] = checkBox->isChecked();
            }
        }
    }
}

void RTGSEntryQuery::checkbox_toggled(bool checked)
{
    for (int pageIndex = 0; pageIndex < totalPages; ++pageIndex) {
        for (int rowIndex = 0; rowIndex < pageSize; ++rowIndex) {
            int row = rowIndex + (pageIndex * pageSize); // 计算当前页的实际行索引
            if (row < totalRows) {
                QCheckBox* checkBox = qobject_cast<QCheckBox*>(ui->tableWidget_2->cellWidget(rowIndex, 0));
                if (checkBox) {
                    // 仅当复选框的状态与全选复选框的状态不同时，才更新复选框的状态
                    if (checkBox->isChecked() != checked) {
                        checkBox->setChecked(checked);
                    }
                    checkboxStateMap[row] = checked;
                }
            }
        }
    }
}

void RTGSEntryQuery::updateTableDisplay()
{
    int startRow = currentPage * pageSize; // 当前页的起始行
    int endRow = qMin(startRow + pageSize, totalRows); // 当前页的结束行
    int numRows = endRow - startRow; // 当前页的行数

    QString dbName = "database.db";
    QString dbPath = QCoreApplication::applicationDirPath() + "./" + dbName;  // Use a relative path
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dbPath);

    if (!database.open()) {
        QMessageBox::critical(nullptr, "错误", "无法打开数据库：" + database.lastError().text());
        return;
    }
    ui->tableWidget_2->clearContents(); // 清空表格内容
    ui->tableWidget_2->setRowCount(numRows);
    // 执行查询
    QSqlQuery query;
    query.prepare("SELECT * FROM entering LIMIT :startRow, :numRows");
    query.bindValue(":startRow", startRow);
    query.bindValue(":numRows", numRows);
    if (query.exec()) {
        int rowIndex = 0; // 当前页内的行索引

        while (query.next()) {
            QCheckBox *checkBox = new QCheckBox();
            QHBoxLayout *layoutCheckBox = new QHBoxLayout();
            QWidget *widget = new QWidget(ui->tableWidget_2);
            layoutCheckBox->addWidget(checkBox);
            layoutCheckBox->setMargin(0);
            layoutCheckBox->setAlignment(checkBox, Qt::AlignCenter);
            widget->setLayout(layoutCheckBox);
            ui->tableWidget_2->setCellWidget(rowIndex, 0, widget);
            connect(checkBox, &QCheckBox::clicked, this, &RTGSEntryQuery::selectRows);
            connect(ui->checkBox, &QCheckBox::toggled, this, &RTGSEntryQuery::checkbox_toggled);
            QTableWidgetItem *item1 = new QTableWidgetItem(query.value(1).toString());
            QTableWidgetItem *item2 = new QTableWidgetItem(query.value(2).toString());
            QTableWidgetItem *item3 = new QTableWidgetItem(query.value(3).toString());
            QTableWidgetItem *item4 = new QTableWidgetItem(query.value(4).toString());
            QTableWidgetItem *item5 = new QTableWidgetItem(query.value(5).toString());
            QTableWidgetItem *item6 = new QTableWidgetItem(query.value(6).toString());
            QTableWidgetItem *item7 = new QTableWidgetItem(query.value(7).toString());
            QTableWidgetItem *item8 = new QTableWidgetItem(query.value(8).toString());
            QTableWidgetItem *emptyItem1 = new QTableWidgetItem("");
            QTableWidgetItem *emptyItem2 = new QTableWidgetItem("");

            ui->tableWidget_2->setItem(rowIndex, 1, emptyItem1);
            ui->tableWidget_2->setItem(rowIndex, 2, item1);
            ui->tableWidget_2->setItem(rowIndex, 3, item2);
            ui->tableWidget_2->setItem(rowIndex, 4, emptyItem2);
            ui->tableWidget_2->setItem(rowIndex, 5, item3);
            ui->tableWidget_2->setItem(rowIndex, 6, item4);
            ui->tableWidget_2->setItem(rowIndex, 7, item5);
            ui->tableWidget_2->setItem(rowIndex, 8, item6);
            ui->tableWidget_2->setItem(rowIndex, 9, item7);
            ui->tableWidget_2->setItem(rowIndex, 10, item8);
            emptyItem1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            emptyItem2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item6->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item7->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item8->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            // 根据checkboxStateMap设置勾选状态
            if(currentPage + 1 != totalPages) {
                int actualRowIndex = startRow + pageSize;
                if (checkboxStateMap.contains(actualRowIndex) && checkboxStateMap[actualRowIndex]) {
                    checkBox->setChecked(true);
                } else {
                    checkBox->setChecked(false);
                }
            } else {
                int actualRowIndex = startRow + rowIndex;
                if (checkboxStateMap.contains(actualRowIndex) && checkboxStateMap[actualRowIndex]) {
                    checkBox->setChecked(true);
                } else {
                    checkBox->setChecked(false);
                }
            }

            ++rowIndex;
        }
        // 删除空行
        removeEmptyRows(ui->tableWidget_2);
    } else {
        QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
        return;
    }

    // 关闭数据库连接
    database.close();
    QString text1 = "当前页记录数:";
    QString text2 = "当前页码:";
    QString text3 = "共计:";
    QString styledText1 = "<font color='black'>" + text1 + "</font>" + "<font color='red'>" + QString::number(numRows) + "</font>";
    QString styledText2 = "<font color='black'>" + text2 + "</font>" + "<font color='red'>" + QString::number(currentPage + 1) + "</font>" + "<font color='black'>" + "/" + "</font>" + "<font color='red'>" + QString::number(totalPages) + "</font>";
    QString styledText3 = "<font color='black'>" + text3 + "</font>" + "<font color='red'>" + QString::number(totalRows) + "</font>" + "<font color='black'>" + "条记录" + "</font>";

    ui->textEdit_2->setHtml(styledText1);
    ui->textEdit_3->setHtml(styledText2);
    ui->textEdit_6->setHtml(styledText3);
}


void RTGSEntryQuery::previousPageButton_clicked()
{
    if (currentPage > 0) {
        currentPage--;
        updateTableDisplay();
    }
}

void RTGSEntryQuery::nextPageButton_clicked()
{
    if (currentPage < totalPages - 1) {
        currentPage++;
        updateTableDisplay();
    }
}

void RTGSEntryQuery::removeEmptyRows(QTableWidget *tableWidget) {
    for (int row = tableWidget->rowCount() - 1; row >= 0; --row) {
        bool isEmptyRow = true;
        for (int column = 0; column < tableWidget->columnCount(); ++column) {
            QTableWidgetItem *item = tableWidget->item(row, column);
            if (item && !item->text().isEmpty()) {
                isEmptyRow = false;
                break;
            }
        }
        if (isEmptyRow) {
            tableWidget->removeRow(row);
            //            totalRows--;
        }
    }
}

void RTGSEntryQuery::siftToData() {

    QString securities_account = ui->lineEdit_7->text();
    QString account = ui->lineEdit_8->text();
    QString code = ui->lineEdit_9->text();
    if(securities_account != NULL || account != NULL || code != NULL) {
        QString dbName = "database.db";
        QString dbPath = QCoreApplication::applicationDirPath() + "./" + dbName;  // Use a relative path
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(dbPath);

        if (!database.open()) {
            QMessageBox::critical(nullptr, "错误", "无法打开数据库：" + database.lastError().text());
            return;
        }

        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM entering");
        if (query.exec()) {
            if (query.next()) {
                totalRows = query.value(0).toInt();
            }
            // 删除空行
        } else {
            QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
            return;
        }
        currentPage = 0,
        pageSize = 50,
        totalPages = ((totalRows + pageSize - 1) / pageSize);
        int startRow = 0 * pageSize; // 当前页的起始行
        int endRow = qMin(startRow + pageSize, totalRows); // 当前页的结束行
        int numRows = endRow - startRow; // 当前页的行数
        int judgeAccount = 0;
        int judgeSecurities_account = 0;
        int judgeCode = 0;

        ui->tableWidget_2->clearContents(); // 清空表格内容
        ui->tableWidget_2->setRowCount(numRows);
        // 执行查询
//        query.prepare("SELECT * FROM entering WHERE account = :account AND securities_account = :securities_account AND code = :code LIMIT :startRow, :numRows");
//        query.bindValue(":account", account);
//        query.bindValue(":securities_account", securities_account);
//        query.bindValue(":code", code);
//        query.bindValue(":startRow", startRow);
//        query.bindValue(":numRows", numRows);
        QString queryString = "SELECT * FROM entering WHERE 1=1";
        if (account != NULL) {
            queryString += " AND account = :account";
            judgeAccount = 1;
        }

        if (securities_account != NULL) {
            queryString += " AND securities_account = :securities_account";
            judgeSecurities_account = 1;
        }

        if (code != NULL) {
            queryString += " AND code = :code";
            judgeCode = 1;
        }

        queryString += " LIMIT :startRow, :numRows";
        query.prepare(queryString);
        if(judgeAccount) {
            query.bindValue(":account", account);
        }
        if(judgeSecurities_account) {
            query.bindValue(":securities_account", securities_account);
        }
        if(judgeCode) {
            query.bindValue(":code", code);
        }
        query.bindValue(":startRow", startRow);
        query.bindValue(":numRows", numRows);
        qDebug() << queryString;

        if (query.exec()) {
            int rowIndex = 0; // 当前页内的行索引

            while (query.next()) {
                QCheckBox *checkBox = new QCheckBox();
                QHBoxLayout *layoutCheckBox = new QHBoxLayout();
                QWidget *widget = new QWidget(ui->tableWidget_2);
                layoutCheckBox->addWidget(checkBox);
                layoutCheckBox->setMargin(0);
                layoutCheckBox->setAlignment(checkBox, Qt::AlignCenter);
                widget->setLayout(layoutCheckBox);
                ui->tableWidget_2->setCellWidget(rowIndex, 0, widget);
                connect(checkBox, &QCheckBox::clicked, this, &RTGSEntryQuery::selectRows);
                connect(ui->checkBox, &QCheckBox::toggled, this, &RTGSEntryQuery::checkbox_toggled);
                QTableWidgetItem *item1 = new QTableWidgetItem(query.value(1).toString());
                QTableWidgetItem *item2 = new QTableWidgetItem(query.value(2).toString());
                QTableWidgetItem *item3 = new QTableWidgetItem(query.value(3).toString());
                QTableWidgetItem *item4 = new QTableWidgetItem(query.value(4).toString());
                QTableWidgetItem *item5 = new QTableWidgetItem(query.value(5).toString());
                QTableWidgetItem *item6 = new QTableWidgetItem(query.value(6).toString());
                QTableWidgetItem *item7 = new QTableWidgetItem(query.value(7).toString());
                QTableWidgetItem *item8 = new QTableWidgetItem(query.value(8).toString());
                QTableWidgetItem *emptyItem1 = new QTableWidgetItem("");
                QTableWidgetItem *emptyItem2 = new QTableWidgetItem("");

                ui->tableWidget_2->setItem(rowIndex, 1, emptyItem1);
                ui->tableWidget_2->setItem(rowIndex, 2, item1);
                ui->tableWidget_2->setItem(rowIndex, 3, item2);
                ui->tableWidget_2->setItem(rowIndex, 4, emptyItem2);
                ui->tableWidget_2->setItem(rowIndex, 5, item3);
                ui->tableWidget_2->setItem(rowIndex, 6, item4);
                ui->tableWidget_2->setItem(rowIndex, 7, item5);
                ui->tableWidget_2->setItem(rowIndex, 8, item6);
                ui->tableWidget_2->setItem(rowIndex, 9, item7);
                ui->tableWidget_2->setItem(rowIndex, 10, item8);
                emptyItem1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                emptyItem2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item6->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item7->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item8->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

                // 根据checkboxStateMap设置勾选状态
                if(currentPage + 1 != totalPages) {
                    int actualRowIndex = startRow + pageSize;
                    if (checkboxStateMap.contains(actualRowIndex) && checkboxStateMap[actualRowIndex]) {
                        checkBox->setChecked(true);
                    } else {
                        checkBox->setChecked(false);
                    }
                } else {
                    int actualRowIndex = startRow + rowIndex;
                    if (checkboxStateMap.contains(actualRowIndex) && checkboxStateMap[actualRowIndex]) {
                        checkBox->setChecked(true);
                    } else {
                        checkBox->setChecked(false);
                    }
                }
                ++rowIndex;
            }
            // 删除空行
            removeEmptyRows(ui->tableWidget_2);
            QString text1 = "当前页记录数:";
            QString text2 = "当前页码:";
            QString text3 = "共计:";
            QString styledText1 = "<font color='black'>" + text1 + "</font>" + "<font color='red'>" + QString::number(numRows) + "</font>";
            QString styledText2 = "<font color='black'>" + text2 + "</font>" + "<font color='red'>" + QString::number(currentPage + 1) + "</font>" + "<font color='black'>" + "/" + "</font>" + "<font color='red'>" + QString::number(totalPages) + "</font>";
            QString styledText3 = "<font color='black'>" + text3 + "</font>" + "<font color='red'>" + QString::number(totalRows) + "</font>" + "<font color='black'>" + "条记录" + "</font>";

            ui->textEdit_2->setHtml(styledText1);
            ui->textEdit_3->setHtml(styledText2);
            ui->textEdit_6->setHtml(styledText3);
        } else {
            QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
            return;
        }

        // 关闭数据库连接
        database.close();
    } else {
        currentPage = 0,
        pageSize = 50,
        totalPages = ((totalRows + pageSize - 1) / pageSize);
        updateTableDisplay();
    }
}

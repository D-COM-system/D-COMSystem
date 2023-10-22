#include "MaximumAmountRegularDeclaration.h"
#include "ui_MaximumAmountRegularDeclaration.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

MaximumAmountRegularDeclaration::MaximumAmountRegularDeclaration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaximumAmountRegularDeclaration)
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
    query.prepare("SELECT COUNT(*) FROM record");
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

    ui->widget_4->setVisible(true);
    // 设置第一个按钮选中状态
    ui->pushButton_5->setChecked(true);

    // 保存第一个按钮的原始样式
    QString originalStyle = ui->pushButton_5->styleSheet();

    // 设置第一个按钮的样式
    QString selectedStyle = "background-color: #1a70ab;color: #ffffff;"; // 设置选中时的背景色
    ui->pushButton_5->setStyleSheet(selectedStyle);

    // 设置其他按钮的样式
    ui->pushButton_11->setStyleSheet(originalStyle);
    ui->pushButton_12->setStyleSheet(originalStyle);
    ui->pushButton_13->setStyleSheet(originalStyle);
    ui->pushButton_14->setStyleSheet(originalStyle);
    ui->pushButton_15->setStyleSheet(originalStyle);
    QStringList headerLabels;
    headerLabels << "状态" << "机构代码" << "机构名称" << "净资本（百万元）" << "合计资产总额（百万元）"
                 << "最高额度（百万元）" << "控制类别" << "录入人" << "录入时间";
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
    // 隐藏左侧的行号框
    updateTableDisplay();
    ui->tableWidget_2->verticalHeader()->setVisible(false);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &MaximumAmountRegularDeclaration::previousPageButton_clicked);
    connect(ui->pushButton_10, &QPushButton::clicked, this, &MaximumAmountRegularDeclaration::nextPageButton_clicked);
    connect(ui->pushButton_17, &QPushButton::clicked, this, &MaximumAmountRegularDeclaration::goToImport);
}

MaximumAmountRegularDeclaration::~MaximumAmountRegularDeclaration()
{
    delete ui;
}

void MaximumAmountRegularDeclaration::updateTableDisplay()
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
    query.prepare("SELECT * FROM record ORDER BY inputTime DESC LIMIT :startRow, :numRows");
    query.bindValue(":startRow", startRow);
    query.bindValue(":numRows", numRows);
    if (query.exec()) {
        int rowIndex = 0; // 当前页内的行索引

        while (query.next()) {
            QString stateResult = query.value(1).toString();
            QString institutionCodeResult = query.value(2).toString();
            QString institutionNameResult = query.value(3).toString();
            QString TotalAssetsResult = query.value(4).toString();
            QString MaxAccountResult = query.value(5).toString();
            QString ControlClassResult = query.value(6).toString();
            QString dataEntryClerkResult = query.value(7).toString();
            QString inputTimeResult = query.value(8).toString();
            QTableWidgetItem *item1 = new QTableWidgetItem(stateResult);
            QTableWidgetItem *item2 = new QTableWidgetItem(institutionCodeResult);
            QTableWidgetItem *item3 = new QTableWidgetItem(institutionNameResult);
            QTableWidgetItem *item4 = new QTableWidgetItem(TotalAssetsResult);
            QTableWidgetItem *item5 = new QTableWidgetItem(MaxAccountResult);
            QTableWidgetItem *item6 = new QTableWidgetItem(ControlClassResult);
            QTableWidgetItem *item7 = new QTableWidgetItem(dataEntryClerkResult);
            QTableWidgetItem *item8 = new QTableWidgetItem(inputTimeResult);
            QTableWidgetItem *emptyItem = new QTableWidgetItem("");

            ui->tableWidget_2->setItem(rowIndex, 0, item1);
            ui->tableWidget_2->setItem(rowIndex, 1, item2);
            ui->tableWidget_2->setItem(rowIndex, 2, item3);
            ui->tableWidget_2->setItem(rowIndex, 3, emptyItem);
            ui->tableWidget_2->setItem(rowIndex, 4, item4);
            ui->tableWidget_2->setItem(rowIndex, 5, item5);
            ui->tableWidget_2->setItem(rowIndex, 6, item6);
            ui->tableWidget_2->setItem(rowIndex, 7, item7);
            ui->tableWidget_2->setItem(rowIndex, 8, item8);
            emptyItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item6->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item7->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item8->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

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

void MaximumAmountRegularDeclaration::removeEmptyRows(QTableWidget *tableWidget) {
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

void MaximumAmountRegularDeclaration::previousPageButton_clicked()
{
    if (currentPage > 0) {
        currentPage--;
        updateTableDisplay();
    }
}

void MaximumAmountRegularDeclaration::nextPageButton_clicked()
{
    if (currentPage < totalPages - 1) {
        currentPage++;
        updateTableDisplay();
    }
}

void MaximumAmountRegularDeclaration::goToImport()
{
    emit informationReturned();
}

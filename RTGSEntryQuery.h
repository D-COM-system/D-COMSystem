#ifndef RTGSENTRYQUERY_H
#define RTGSENTRYQUERY_H

#include <QWidget>
#include <QVector>
#include <QMap>
#include <QTableWidget>
#include <QHeaderView>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>


namespace Ui {
class RTGSEntryQuery;
}

class RTGSEntryQuery : public QWidget
{
    Q_OBJECT

public:
    explicit RTGSEntryQuery(QWidget *parent = nullptr);
    ~RTGSEntryQuery();

public:
    Ui::RTGSEntryQuery *ui;
    int currentPage; // 当前页码
    int pageSize; // 每页显示的行数
    int totalRows; // 总行数
    int totalPages; // 总页数
    QVector<bool> boolArray;
    QMap<int, bool> checkboxStateMap;
    void updateTableDisplay();
    void removeEmptyRows(QTableWidget *tableWidget);

private slots:
    void selectRows();
    void previousPageButton_clicked();
    void nextPageButton_clicked();
    void checkbox_toggled(bool checked);
    void siftToData();

};

#endif // RTGSENTRYQUERY_H

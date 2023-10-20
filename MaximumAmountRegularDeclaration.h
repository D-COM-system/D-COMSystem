#ifndef MAXIMUMAMOUNTREGULARDECLARATION_H
#define MAXIMUMAMOUNTREGULARDECLARATION_H

#include "ImportOfDeclarations.h"
#include "ui_ImportOfDeclarations.h"

#include <QWidget>
#include <QTableWidget>

namespace Ui {
class MaximumAmountRegularDeclaration;
}

class MaximumAmountRegularDeclaration : public QWidget
{
    Q_OBJECT

public:
    explicit MaximumAmountRegularDeclaration(QWidget *parent = nullptr);
    ~MaximumAmountRegularDeclaration();

public:
    Ui::MaximumAmountRegularDeclaration *ui;
    MaximumAmountRegularDeclaration *maximumAmountRegularDeclaration;
    ImportOfDeclarations *importofdeclarations;
    int currentPage; // 当前页码
    int pageSize; // 每页显示的行数
    int totalRows; // 总行数
    int totalPages; // 总页数
    void updateTableDisplay();
    void removeEmptyRows(QTableWidget *tableWidget);

private slots:
    void previousPageButton_clicked();
    void nextPageButton_clicked();
    void goToImport();


signals:
    void informationReturned();
};

#endif // MAXIMUMAMOUNTREGULARDECLARATION_H

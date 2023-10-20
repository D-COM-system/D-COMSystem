#ifndef FUNDBALANCEQUERY_H
#define FUNDBALANCEQUERY_H

#include <QWidget>

namespace Ui {
class FundBalanceQuery;
}

class FundBalanceQuery : public QWidget
{
    Q_OBJECT

public:
    explicit FundBalanceQuery(QWidget *parent = nullptr);
    ~FundBalanceQuery();

public:
    Ui::FundBalanceQuery *ui;

private slots:
    void comboBoxAddItem();
    void siftToData();
};

#endif // FUNDBALANCEQUERY_H

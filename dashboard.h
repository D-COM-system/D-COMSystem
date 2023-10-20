#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "globalData.h"
#include "FundBalanceQuery.h"
#include "ui_FundBalanceQuery.h"
#include "RTGSEntryQuery.h"
#include "ui_RTGSEntryQuery.h"
#include "MaximumAmountRegularDeclaration.h"
#include "ui_MaximumAmountRegularDeclaration.h"
#include "ImportOfDeclarations.h"
#include "ui_ImportOfDeclarations.h"

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class dashboard; }
QT_END_NAMESPACE

class dashboard : public QWidget
{
    Q_OBJECT

public:
    dashboard(QWidget *parent = nullptr);
    ~dashboard();

public:
    Ui::dashboard *ui;
    FundBalanceQuery *fundbalancequery;
    RTGSEntryQuery *RTGSentryquery;
    MaximumAmountRegularDeclaration *maximumamountregulardeclaration;
    ImportOfDeclarations *importofdeclarations;


private slots:
    void recivelogin();
    void updateTime();
    void initializeDashboard();
    void createFundMenu(QTreeWidget* treeWidget);
    void createRTGSMenu(QTreeWidget* treeWidget);
    void onTreeWidgetItemClicked(QTreeWidgetItem* item, int column);
    void reDashboard();

public slots:
    void handleReturnedInformation();
    void handleBackInformation();

protected:
    void showEvent(QShowEvent *event) override;
    bool eventFilter(QObject* object, QEvent* event) override;
};

#endif // DASHBOARD_H

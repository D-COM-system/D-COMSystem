#ifndef IMPORTOFDECLARATIONS_H
#define IMPORTOFDECLARATIONS_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>

namespace Ui {
class ImportOfDeclarations;
}

class ImportOfDeclarations : public QWidget
{
    Q_OBJECT

public:
    explicit ImportOfDeclarations(QWidget *parent = nullptr);
    ~ImportOfDeclarations();

public:
    Ui::ImportOfDeclarations *ui;

private slots:
    void backToImport();
    void chooseFile();
    void importFile();
    void resetText();
    void closeMessageWindow();

signals:
    void informationBack();
};

#endif // IMPORTOFDECLARATIONS_H

#ifndef DLGIMPORTGAERVERLAUF_H
#define DLGIMPORTGAERVERLAUF_H

#include <QDialog>

namespace Ui {
class DlgImportGaerverlauf;
}

class DlgImportGaerverlauf : public QDialog
{
    Q_OBJECT

public:
    explicit DlgImportGaerverlauf(QWidget *parent = nullptr);
    ~DlgImportGaerverlauf();

private slots:
    void on_btnDownload_clicked();


private:
    Ui::DlgImportGaerverlauf *ui;

public:
    QStringList data;


};

#endif // DLGIMPORTGAERVERLAUF_H

#include "wdgrast.h"
#include "ui_wdgrast.h"
#include "brauhelfer.h"

extern Brauhelfer* bh;

WdgRast::WdgRast(int index, QWidget *parent) :
	QWidget(parent),
    ui(new Ui::WdgRast),
    mIndex(index),
    mEnabled(true)
{
	ui->setupUi(this);

    checkEnabled(true);
    updateValues();
    connect(bh->sud()->modelRasten(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&)), this, SLOT(updateValues()));
}

WdgRast::~WdgRast()
{
	delete ui;
}

bool WdgRast::isEnabled() const
{
    return mEnabled;
}

QVariant WdgRast::data(const QString &fieldName) const
{
    return bh->sud()->modelRasten()->data(mIndex, fieldName);
}

bool WdgRast::setData(const QString &fieldName, const QVariant &value)
{
    return bh->sud()->modelRasten()->setData(mIndex, fieldName, value);
}

void WdgRast::checkEnabled(bool force)
{
    bool enabled = !bh->sud()->getBierWurdeGebraut();
    if (enabled == mEnabled && !force)
        return;

    mEnabled = enabled;
    if (mEnabled)
    {
        ui->cbRast->clear();
        ui->cbRast->addItem(tr("Gummirast (35°-40°)"));
        ui->cbRast->addItem(tr("Weizenrast (45°)"));
        ui->cbRast->addItem(tr("Eiweißrast (57°)"));
        ui->cbRast->addItem(tr("Maltoserast (60°-65°)"));
        ui->cbRast->addItem(tr("Kombirast (66°-69°)"));
        ui->cbRast->addItem(tr("Verzuckerung (70°-75°)"));
        ui->cbRast->addItem(tr("Abmaischen (78°)"));
        ui->cbRast->setEditable(true);
        ui->cbRast->setEnabled(true);
        ui->cbRast->setCurrentIndex(-1);
        ui->btnLoeschen->setVisible(true);
        ui->tbTemp->setReadOnly(false);
        ui->tbDauer->setReadOnly(false);
    }
    else
    {
        ui->cbRast->clear();
        ui->cbRast->addItem(data("Name").toString());
        ui->cbRast->setEditable(false);
        ui->cbRast->setEnabled(false);
        ui->cbRast->setCurrentIndex(-1);
        ui->btnLoeschen->setVisible(false);
        ui->tbTemp->setReadOnly(true);
        ui->tbDauer->setReadOnly(true);
    }
}

void WdgRast::updateValues()
{
    checkEnabled(false);

    if (!ui->cbRast->hasFocus())
        ui->cbRast->setCurrentText(data("Name").toString());
    if (!ui->tbTemp->hasFocus())
        ui->tbTemp->setValue(data("Temp").toInt());
    if (!ui->tbDauer->hasFocus())
        ui->tbDauer->setValue(data("Dauer").toInt());
}

void WdgRast::on_cbRast_currentTextChanged(const QString &text)
{
    if (ui->cbRast->hasFocus())
        setData("Name", text);
}

void WdgRast::on_cbRast_currentIndexChanged(int index)
{
    if (ui->cbRast->hasFocus())
    {
        setData("Name", ui->cbRast->itemText(index));
        switch (index)
        {
        case 0: //Gummirast
            setData("Temp", 38);
            setData("Dauer", 60);
            break;
        case 1: //Weizenrast
            setData("Temp", 45);
            setData("Dauer", 15);
            break;
        case 2: //Eiweißrast
            setData("Temp", 57);
            setData("Dauer", 10);
            break;
        case 3: //Maltoserast
            setData("Temp", 63);
            setData("Dauer", 35);
            break;
        case 4: //Kombirast
            setData("Temp", 67);
            setData("Dauer", 60);
            break;
        case 5: //Verzuckerung
            setData("Temp", 72);
            setData("Dauer", 20);
            break;
        case 6: //Abmaischen
            setData("Temp", 78);
            setData("Dauer", 0);
        }
        setFocus();
        bh->sud()->modelRasten()->sort();
    }
}

void WdgRast::on_tbTemp_valueChanged(int value)
{
    if (ui->tbTemp->hasFocus())
        setData("Temp", value);
}

void WdgRast::on_tbTemp_editingFinished()
{
    setFocus();
    bh->sud()->modelRasten()->sort();
}

void WdgRast::on_tbDauer_valueChanged(int value)
{
    if (ui->tbDauer->hasFocus())
        setData("Dauer", value);
}

void WdgRast::on_btnLoeschen_clicked()
{
    bh->sud()->modelRasten()->removeRow(mIndex);
}

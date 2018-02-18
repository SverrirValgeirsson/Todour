#include <QFileDialog>
#include <QFontDialog>
#include <QColorDialog>
#include <QPalette>
#include <QRgb>

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "globals.h"

#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    QSettings settings;
    ui->lineEdit->setText(settings.value("directory").toString());
    ui->lineEdit_2->setText(settings.value("inactive").toString());
    ui->cb_autorefresh->setChecked(settings.value("autorefresh").toBool());
    ui->cb_separate->setChecked(settings.value("separateinactive").toBool());
    ui->cb_dates->setChecked(settings.value("dates").toBool());
    ui->cb_liveSearch->setChecked(settings.value("liveSearch").toBool());
    // Handle the fonts
    activecolor=QColor::fromRgba(settings.value("activecolor",DEFAULT_ACTIVE_COLOR).toUInt());
    inactivecolor=QColor::fromRgba(settings.value("inactivecolor",DEFAULT_INACTIVE_COLOR).toUInt());
    activefont.fromString(settings.value("activefont").toString());
    inactivefont.fromString(settings.value("inactivefont").toString());
    updateFonts();
    refresh=false;
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_rejected()
{
    this->close();
}

void SettingsDialog::on_buttonBox_accepted()
{
    QSettings settings;
    QString dir=ui->lineEdit->text();
    if(!dir.endsWith('/')){
        dir.append('/');
    }
    settings.setValue("directory",dir);
    settings.setValue("inactive",ui->lineEdit_2->text());
    settings.setValue("autorefresh",ui->cb_autorefresh->isChecked());
    settings.setValue("separateinactive",ui->cb_separate->isChecked());
    settings.setValue("dates",ui->cb_dates->isChecked());
    settings.setValue("liveSearch",ui->cb_liveSearch->isChecked());
    settings.setValue("inactivecolor",inactivecolor.rgba());
    settings.setValue("activecolor",activecolor.rgba());
    settings.setValue("inactiveFont",inactivefont.toString());
    settings.setValue("activeFont",activefont.toString());
    refresh=true;
    this->close();
}

void SettingsDialog::on_btnDirBrowse_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,"Select Directory",ui->lineEdit->text());
    if(!dir.isEmpty())
        ui->lineEdit->setText(dir);
}

void SettingsDialog::on_pb_fontDlgActive_clicked()
{
    activefont=QFontDialog::getFont(0,ui->le_activeFont->font());
    updateFonts();
}

void SettingsDialog::updateFonts(){

    QPalette p;
    ui->le_activeFont->setFont(activefont);
    p.setColor(QPalette::Text,activecolor);
    ui->le_activeFont->setPalette(p);
    p.setColor(QPalette::Text,inactivecolor);
    ui->le_InactiveFont->setPalette(p);
    ui->le_InactiveFont->setFont(inactivefont);
}

void SettingsDialog::on_pb_fontDlgInactive_clicked()
{
    inactivefont=QFontDialog::getFont(0,ui->le_InactiveFont->font());
    updateFonts();
}

void SettingsDialog::on_pb_colorDlgActive_clicked()
{
    activecolor.setRgba(QColorDialog::getRgba());
    updateFonts();
}

void SettingsDialog::on_pb_colorDlgInactive_clicked()
{
    inactivecolor.setRgba(QColorDialog::getRgba());
    updateFonts();
}

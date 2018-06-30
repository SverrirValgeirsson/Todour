#include <QFileDialog>
#include <QFontDialog>
#include <QColorDialog>
#include <QPalette>
#include <QRgb>

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "globals.h"
#include "def.h"

#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    QSettings settings;
    ui->lineEdit->setText(settings.value(SETTINGS_DIRECTORY,DEFAULT_DIRECTORY).toString());
    ui->lineEdit_2->setText(settings.value(SETTINGS_INACTIVE,DEFAULT_INACTIVE).toString());
    ui->cb_autorefresh->setChecked(settings.value(SETTINGS_AUTOREFRESH,DEFAULT_AUTOREFRESH).toBool());
    ui->cb_separate->setChecked(settings.value(SETTINGS_SEPARATE_INACTIVES,DEFAULT_SEPARATE_INACTIVES).toBool());
    ui->cb_deletedfile->setChecked(settings.value(SETTINGS_DELETED_FILE,DEFAULT_DELETED_FILE).toBool());
    ui->cb_threshold->setChecked(settings.value(SETTINGS_THRESHOLD,DEFAULT_THRESHOLD).toBool());
    ui->cb_threshold_label->setChecked(settings.value(SETTINGS_THRESHOLD_LABELS,DEFAULT_THRESHOLD_LABELS).toBool());
    ui->cb_threshold_inactive->setChecked(settings.value(SETTINGS_THRESHOLD_INACTIVE,DEFAULT_THRESHOLD_INACTIVE).toBool());
    ui->cb_dates->setChecked(settings.value(SETTINGS_DATES,DEFAULT_DATES).toBool());
    ui->cb_showdates->setChecked(settings.value(SETTINGS_SHOW_DATES,DEFAULT_SHOW_DATES).toBool());
    ui->cb_liveSearch->setChecked(settings.value(SETTINGS_LIVE_SEARCH,DEFAULT_LIVE_SEARCH).toBool());
    ui->cb_hotKey->setChecked(settings.value(SETTINGS_HOTKEY_ENABLE,DEFAULT_HOTKEY_ENABLE).toBool());
    // Handle the fonts
    activecolor=QColor::fromRgba(settings.value(SETTINGS_ACTIVE_COLOR,DEFAULT_ACTIVE_COLOR).toUInt());
    inactivecolor=QColor::fromRgba(settings.value(SETTINGS_INACTIVE_COLOR,DEFAULT_INACTIVE_COLOR).toUInt());
    activefont.fromString(settings.value(SETTINGS_ACTIVE_FONT).toString());
    inactivefont.fromString(settings.value(SETTINGS_INACTIVE_FONT).toString());
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
    settings.setValue(SETTINGS_DIRECTORY,dir);
    settings.setValue(SETTINGS_INACTIVE,ui->lineEdit_2->text());
    settings.setValue(SETTINGS_AUTOREFRESH,ui->cb_autorefresh->isChecked());
    settings.setValue(SETTINGS_SEPARATE_INACTIVES,ui->cb_separate->isChecked());
    settings.setValue(SETTINGS_DATES,ui->cb_dates->isChecked());
    settings.setValue(SETTINGS_SHOW_DATES,ui->cb_showdates->isChecked());
    settings.setValue(SETTINGS_LIVE_SEARCH,ui->cb_liveSearch->isChecked());
    settings.setValue(SETTINGS_INACTIVE_COLOR,inactivecolor.rgba());
    settings.setValue(SETTINGS_ACTIVE_COLOR,activecolor.rgba());
    settings.setValue(SETTINGS_INACTIVE_FONT,inactivefont.toString());
    settings.setValue(SETTINGS_ACTIVE_FONT,activefont.toString());
    settings.setValue(SETTINGS_HOTKEY_ENABLE,ui->cb_hotKey->isChecked());
    settings.setValue(SETTINGS_DELETED_FILE,ui->cb_deletedfile->isChecked());
    settings.setValue(SETTINGS_THRESHOLD,ui->cb_threshold->isChecked());
    settings.setValue(SETTINGS_THRESHOLD_LABELS,ui->cb_threshold_label->isChecked());
    settings.setValue(SETTINGS_THRESHOLD_INACTIVE,ui->cb_threshold_inactive->isChecked());
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

void SettingsDialog::on_cb_hotKey_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
}

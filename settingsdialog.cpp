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
    ui->cb_due_as_threshold->setChecked(settings.value(SETTINGS_DUE_AS_THRESHOLD,DEFAULT_DUE_AS_THRESHOLD).toBool());
    ui->cb_dates->setChecked(settings.value(SETTINGS_DATES,DEFAULT_DATES).toBool());
    ui->cb_showdates->setChecked(settings.value(SETTINGS_SHOW_DATES,DEFAULT_SHOW_DATES).toBool());
    ui->cb_liveSearch->setChecked(settings.value(SETTINGS_LIVE_SEARCH,DEFAULT_LIVE_SEARCH).toBool());
    ui->cb_hotKey->setChecked(settings.value(SETTINGS_HOTKEY_ENABLE,DEFAULT_HOTKEY_ENABLE).toBool());
    ui->cb_tray_icon->setChecked(settings.value(SETTINGS_TRAY_ENABLED,DEFAULT_TRAY_ENABLED).toBool());
    ui->cb_CheckUpdates->setChecked(settings.value(SETTINGS_CHECK_UPDATES,DEFAULT_CHECK_UPDATES).toBool());
    ui->cb_due->setChecked(settings.value(SETTINGS_DUE,DEFAULT_DUE).toBool());
    ui->sb_due_warning->setValue(settings.value(SETTINGS_DUE_WARNING,DEFAULT_DUE_WARNING).toInt());
    ui->comb_priorities->setCurrentIndex(settings.value(SETTINGS_PRIO_ON_CLOSE, DEFAULT_PRIO_ON_CLOSE).toInt());
    ui->sb_fontSize->setValue(qApp->font().pointSize());
    ui->cb_removeDoublets->setChecked(settings.value(SETTINGS_REMOVE_DOUBLETS,DEFAULT_REMOVE_DOUBLETS).toBool());
    ui->search_not_char->setText(settings.value(SETTINGS_SEARCH_NOT_CHAR,DEFAULT_SEARCH_NOT_CHAR).toChar());
    ui->cb_default_threshold->setCurrentText(settings.value(SETTINGS_DEFAULT_THRESHOLD,DEFAULT_DEFAULT_THRESHOLD).toString());

    // Business days
    QList<int> business_days = settings.value(SETTINGS_BUSINESS_DAYS, QVariant::fromValue(QList<int>())).value<QList<int> >();
    if(business_days.size()==0){
        // Hard code some defaults
        for(int i=DEFAULT_BUSINESS_DAYS_FIRST;i<=DEFAULT_BUSINESS_DAYS_LAST;i++){
            business_days<<i;
        }
    }

    ui->cb_monday->setChecked(business_days.contains(1));
    ui->cb_tuesday->setChecked(business_days.contains(2));
    ui->cb_wednesday->setChecked(business_days.contains(3));
    ui->cb_thursday->setChecked(business_days.contains(4));
    ui->cb_friday->setChecked(business_days.contains(5));
    ui->cb_saturday->setChecked(business_days.contains(6));
    ui->cb_sunday->setChecked(business_days.contains(7));


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
    settings.setValue(SETTINGS_DUE_AS_THRESHOLD,ui->cb_due_as_threshold->isChecked());
    settings.setValue(SETTINGS_TRAY_ENABLED,ui->cb_tray_icon->isChecked());
    settings.setValue(SETTINGS_DUE,ui->cb_due->isChecked());
    settings.setValue(SETTINGS_DUE_WARNING,ui->sb_due_warning->value());
    settings.setValue(SETTINGS_CHECK_UPDATES,ui->cb_CheckUpdates->isChecked());
    settings.setValue(SETTINGS_PRIO_ON_CLOSE,ui->comb_priorities->currentIndex());
    settings.setValue(SETTINGS_DEFAULT_THRESHOLD,ui->cb_default_threshold->currentText());
    settings.setValue(SETTINGS_FONT_SIZE,ui->sb_fontSize->value());
    settings.setValue(SETTINGS_REMOVE_DOUBLETS,ui->cb_removeDoublets->isChecked());
    if(ui->search_not_char->text().size()>0)
        settings.setValue(SETTINGS_SEARCH_NOT_CHAR,ui->search_not_char->text().at(0));

    // Handle business days
    QList<int> business_days;
    if(ui->cb_monday->isChecked())
        business_days<<1;
    if(ui->cb_tuesday->isChecked())
        business_days<<2;
    if(ui->cb_wednesday->isChecked())
        business_days<<3;
    if(ui->cb_thursday->isChecked())
        business_days<<4;
    if(ui->cb_friday->isChecked())
        business_days<<5;
    if(ui->cb_saturday->isChecked())
        business_days<<6;
    if(ui->cb_sunday->isChecked())
        business_days<<7;

    settings.setValue(SETTINGS_BUSINESS_DAYS,QVariant::fromValue(business_days));

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
    QSettings settings;
    QPalette p;
    ui->le_activeFont->setFont(activefont);
    p.setColor(QPalette::Text,activecolor);
    ui->le_activeFont->setPalette(p);
    p.setColor(QPalette::Text,inactivecolor);
    ui->le_InactiveFont->setPalette(p);
    ui->le_InactiveFont->setFont(inactivefont);

    // Handle the colors of the due buttons
    QPalette pal = ui->pb_warningColor->palette();
    pal.setColor(QPalette::ButtonText,QColor::fromRgba(settings.value(SETTINGS_DUE_WARNING_COLOR,DEFAULT_DUE_WARNING_COLOR).toInt()));
    pal.setColor(QPalette::Button,QColor::fromRgba(0xFFFFFFFF));
    ui->pb_warningColor->setPalette(pal);
    ui->pb_warningColor->update();

    pal.setColor(QPalette::ButtonText,QColor::fromRgba(settings.value(SETTINGS_DUE_LATE_COLOR,DEFAULT_DUE_LATE_COLOR).toInt()));
    ui->pb_lateColor->setPalette(pal);
    ui->pb_lateColor->update();
}

void SettingsDialog::on_pb_fontDlgInactive_clicked()
{
    inactivefont=QFontDialog::getFont(0,ui->le_InactiveFont->font());
    updateFonts();
}

void SettingsDialog::on_pb_colorDlgActive_clicked()
{
    activecolor = QColorDialog::getColor();
    updateFonts();
}

void SettingsDialog::on_pb_colorDlgInactive_clicked()
{
    inactivecolor = QColorDialog::getColor();
    updateFonts();
}

void SettingsDialog::on_cb_hotKey_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
}

void SettingsDialog::on_pb_warningColor_clicked()
{
    QSettings settings;
//    QColor c = QColorDialog::getRgba(settings.value(SETTINGS_DUE_WARNING_COLOR,DEFAULT_DUE_WARNING_COLOR).toInt());
    QColor c = QColorDialog::getColor(QColor::fromRgb(settings.value(SETTINGS_DUE_WARNING_COLOR,DEFAULT_DUE_WARNING_COLOR).toInt()),this);
    settings.setValue(SETTINGS_DUE_WARNING_COLOR,c.rgba());
    updateFonts();
}

void SettingsDialog::on_pb_lateColor_clicked()
{
    QSettings settings;
//    QColor c = QColorDialog::getRgba(settings.value(SETTINGS_DUE_LATE_COLOR,DEFAULT_DUE_LATE_COLOR).toInt());
    QColor c = QColorDialog::getColor(QColor::fromRgb(settings.value(SETTINGS_DUE_LATE_COLOR,DEFAULT_DUE_LATE_COLOR).toInt()),this);
    settings.setValue(SETTINGS_DUE_LATE_COLOR,c.rgba());
    updateFonts();

}

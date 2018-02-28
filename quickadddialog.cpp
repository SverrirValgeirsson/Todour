#include "quickadddialog.h"
#include "ui_quickadddialog.h"

QuickAddDialog::QuickAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuickAddDialog)
{
    ui->setupUi(this);
    accepted=false;
}

QuickAddDialog::~QuickAddDialog()
{
    delete ui;
}

void QuickAddDialog::on_buttonBox_accepted()
{
    accepted= true;
    text = ui->lineEdit->text();
}

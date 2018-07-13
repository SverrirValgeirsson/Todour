#include "aboutbox.h"
#include "ui_aboutbox.h"
#include "globals.h"

AboutBox::AboutBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutBox)
{
    ui->setupUi(this);
}



AboutBox::~AboutBox()
{
    delete ui;
}

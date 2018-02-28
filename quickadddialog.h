#ifndef QUICKADDDIALOG_H
#define QUICKADDDIALOG_H

#include <QDialog>

namespace Ui {
class QuickAddDialog;
}

class QuickAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QuickAddDialog(QWidget *parent = 0);
    ~QuickAddDialog();
    bool accepted;
    QString text;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::QuickAddDialog *ui;
};

#endif // QUICKADDDIALOG_H

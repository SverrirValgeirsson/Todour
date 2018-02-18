#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void parse_todotxt();
    ~MainWindow();
    
public slots:
    void fileModified(const QString& str);

private slots:
    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_actionSettings_triggered();

    void on_actionAbout_triggered();

    void on_pushButton_clicked();

    void on_lineEdit_returnPressed();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void dataInModelChanged(QModelIndex i1,QModelIndex i2);

    void on_btn_Alphabetical_toggled(bool checked);

    void on_lineEdit_2_returnPressed();

private:
    void setFileWatch();
    void clearFileWatch();
    void closeEvent(QCloseEvent *ev);
    Ui::MainWindow *ui;
    void saveTableSelection();
    void resetTableSelection();
    void updateSearchResults();
};

#endif // MAINWINDOW_H

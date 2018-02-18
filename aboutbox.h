#ifndef ABOUTBOX_H
#define ABOUTBOX_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#ifdef Q_OS_OSX
    #define VERSION_URL "http://nerdur.com/todour-latest_mac.php"
#elif defined Q_OS_WIN
    #define VERSION_URL "http://nerdur.com/todour-latest_windows.php"
#else
    #define VERSION_URL "http://nerdur.com/todour-latest.php"
#endif


namespace Ui {
class AboutBox;
}

class AboutBox : public QDialog
{
    Q_OBJECT

public:
    explicit AboutBox(QWidget *parent = 0);
    ~AboutBox();
public slots:
    void requestReceived(QNetworkReply* reply);

private:
    Ui::AboutBox *ui;
    void requestPage(QString &s);
};

#endif // ABOUTBOX_H

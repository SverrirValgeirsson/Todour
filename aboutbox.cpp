#include "aboutbox.h"
#include "ui_aboutbox.h"
#include "globals.h"

AboutBox::AboutBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutBox)
{
    QString URL = VERSION_URL;
    requestPage(URL);
    ui->setupUi(this);
}

// Check if there is an update available
void AboutBox::requestPage(QString &s){
    connect(networkaccessmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestReceived(QNetworkReply*)));
    networkaccessmanager->get(QNetworkRequest(QUrl(s)));

}

void AboutBox::requestReceived(QNetworkReply* reply){
    QString replyText;
    //qDebug() << "Recived results from HTTP"<<endl;
    if(reply->error()==QNetworkReply::NoError){

        // Get the http status code
        int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (v >= 200 && v < 300) // Success
        {
            //qDebug() << "Reply OK"<<endl;
            replyText = reply->readAll();
            double latest_version = replyText.toDouble();
            //qDebug()<<latest_version<<endl;
            ui->txtLatestVersion->setText(QString::number(latest_version,'f',2));
        }
    }
    ui->lblCurrentVersion->setText(VER);
    reply->deleteLater();
}


AboutBox::~AboutBox()
{
    delete ui;
}

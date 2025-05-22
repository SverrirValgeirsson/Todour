
#include "version.h"
#include "def.h"
#include <QSettings>
#include <QDebug>


todour_version::~todour_version()
{
    delete networkaccessmanager;
}

todour_version::todour_version()
{
	QSettings settings;
     networkaccessmanager = new QNetworkAccessManager(this);
     qDebug()<<"todour_version created."<<endline;
}

void todour_version::onlineCheck(bool forced)
// This function stats an online check of the version.  This can be triggered manually=forced (from Aboutbox) of automatically.
{
	QSettings settings;
    if(settings.value(SETTINGS_CHECK_UPDATES,!DEFAULT_CHECK_UPDATES).toBool()){
        QString last_check = settings.value(SETTINGS_LAST_UPDATE_CHECK,"").toString();
        if(last_check.isEmpty()){
            // We set this up so that first check will be later, giving users ample time to turn off the feature.
            last_check = QDate::currentDate().toString("yyyy-MM-dd");
            settings.setValue(SETTINGS_LAST_UPDATE_CHECK,last_check);

        }
        QDate lastCheck = QDate::fromString(last_check,"yyyy-MM-dd");
        QDate nextCheck = lastCheck.addDays(7);

 //       qDebug()<<"Last update check date: "<<last_check<<" and next is "<<nextCheck.toString("yyyy-MM-dd")<<endline;
        int daysToNextcheck = QDate::currentDate().daysTo(nextCheck);
        if(forced || daysToNextcheck<0){
            QString URL = VERSION_URL;
 			connect(networkaccessmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestReceived(QNetworkReply*)));
			networkaccessmanager->get(QNetworkRequest(QUrl(URL)));
        }

    }
}

/* TODO   this need rework:
1. change the UI to have only 1 label "LBL_version", which we will change the text.
2. review this function to issue the right text
3. in MainWindow, consider changing the QWidget "version" to a nice notification with timeout.
4. Consider adding a bypass to skip checking nerdur. possible to make the check with GIT?

*/

void todour_version::requestReceived(QNetworkReply* reply){
    QString replyText;
   	QSettings settings;

    // Have a default showing that you are running the latest
    if(reply->error()==QNetworkReply::NoError){

        // Get the http status code
        int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (v >= 200 && v < 300) // Success
        {
            replyText = reply->readAll();
            double latest_version = replyText.toDouble();
            double this_version = QString(VER).toDouble();
            if(latest_version>this_version){
                emit NewVersion("New version available: "+QString::number(latest_version));
            }
            else
            {
            	emit NewVersion("You are up to date.");
            }
            // Update the last checked since we were successful
            settings.setValue(SETTINGS_LAST_UPDATE_CHECK,QDate::currentDate().toString("yyyy-MM-dd"));
        }
    }
    else
    {
                emit NewVersion("Network error.");
    
    }
    reply->deleteLater();
}
 

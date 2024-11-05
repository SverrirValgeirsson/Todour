#ifndef TODOUR_VERSION_H
#define TODOUR_VERSION_H


#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>


#ifdef Q_OS_OSX
    #define VERSION_URL "https://nerdur.com/todour-latest_mac.php"
#elif defined Q_OS_WIN
    #define VERSION_URL "https://nerdur.com/todour-latest_windows.php"
#else
    #define VERSION_URL "https://nerdur.com/todour-latest.php"
#endif



class todour_version: public QObject{
 	Q_OBJECT

	public:
		explicit todour_version();
		~todour_version();
		
		void onlineCheck(bool forced);
		static QString get_version(){ return VER;}

		
	signals:		
	    void NewVersion(QString text);		
	    //void NewVersion();	    
		
	public slots:
    	void requestReceived(QNetworkReply* reply);
	
	private:
    	QNetworkAccessManager* networkaccessmanager;
		void requestPage(QString &s);    

};


#endif /*TODOUR_VERSION_H*/

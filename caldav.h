/* The caldav master class.
  It handles the caldav interface
  */

#ifndef CALDAV_H
#define CALDAV_H

#include <vector>
#include <QString>
#include <QDate>
#include <QFile>
#include <QObject>

#include "task.h"
#include "todo_backend.h"
#include "CalendarClient_CalDAV.h"

using namespace std;

class caldav : public todo_backend
{
public:
    explicit caldav(QObject *parent = 0);
    ~caldav();

	void getAllTask(vector<task*> &output);
//	void clearMonitoring(); //gaetan 5/1/24
	void setMonitoring(bool b, QObject *parent); //gaetan 5/1/24
	void writeRequest(vector<task*>& content, TodoDestination t, bool append);
	void reloadRequest();
	bool isReady();
	QString getType();
protected:
private:
    CalendarClient_CalDAV* pCalendar;
public slots:
 
};

#endif // TODOTXT_H

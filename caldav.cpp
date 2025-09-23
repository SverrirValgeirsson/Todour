#include "caldav.h"
#include "def.h"

//caldav ref: https://sabre.io/dav/building-a-caldav-client/

#include <QSettings>
#include <QDebug>
#include <QUuid>
#include <QList>
	
#include "CalendarClient_CalDAV.h"
#include "SimpleCrypt.h"

#define PWD_CRYPT Q_UINT64_C(0x3A3CF524EC443FB1)

caldav::caldav(QObject *parent) :
	todo_backend(parent)
{
    QSettings settings;

	SimpleCrypt crypto(PWD_CRYPT);
 
    pCalendar = new CalendarClient_CalDAV(this);
 if(settings.value(SETTINGS_CALDAV_URL,"").toString().isEmpty())
 	{
 	emit ConnectionLost();
 	}
 else
 	{
    pCalendar->setHostURL(QUrl(settings.value(SETTINGS_CALDAV_URL,"").toString()));
    pCalendar->setUsername(settings.value(SETTINGS_CALDAV_USERNAME,"").toString());
	QString password = crypto.decryptToString(settings.value(SETTINGS_CALDAV_PASSWORD,"").toString());
    pCalendar->setPassword(password);
//    pCalendar->setDisplayName("Name");
    connect(pCalendar, SIGNAL(eventsUpdated()), this, SIGNAL(DataAvailable()));
    }
}

caldav::~caldav()
/* */
{
}

bool caldav::isReady()
/* */
{
	qDebug()<<"caldav::isReady()"<<endline;
	return false; //not working yet.
}

void caldav::reloadRequest()
{
	pCalendar->startSynchronization();
}

void caldav::writeRequest(vector<task*>& content, TodoDestination t, bool append)
/* Writes a set of tasks (vector) to the server.
#TODO optimize the conversion from task > CalendarEvent > exploded html
*/
{
    qDebug()<<"caldav::writeB("<<t<<"). append="<<append<<endline;
	// #TODO  consider the append. How is it working in reality?
	CalendarEvent* evt = new CalendarEvent(pCalendar);
	for (vector<task*>::iterator i=content.begin();i!=content.end();i++)
		{
//		  evt->setColor(other.m_Color);
//		  evt->setCalendarName(other.m_calendarName);
		  evt->setName((*i)->getEditText());
//		  evt->setLocation(other.m_Location);
//		  evt->setDescription(other.m_Description);
//		  evt->setStartDateTime(other.m_StartDateTime);
//		  evt->setEndDateTime(other.m_EndDateTime);
//		  evt->setCategories(other.m_Categories);
//		  evt->setExdates(other.m_Exdates);
//		  evt->setRRULE(other.m_RRULE);
//		  evt->setIsCanceled(other.m_IsCanceled);
		  evt->setUID((*i)->getTuid().toString(QUuid::WithoutBraces));
//		  evt->setHREF(other.m_HREF);
		  evt->setCalendarPointer(pCalendar);
		  evt->setType(E_VTODO);
		
		  pCalendar->saveEvent(evt);
		  evt->reset();
		  }
//    out.setEncoding(QStringConverter::Utf8);
//    for(vector<task*>::iterator i=content.begin(); i!=content.end(); i++)
//         out << (*i)->toSaveString() << "\n";

	emit DataSaved();
    return	;
}


void caldav::getAllTask(vector<task*> &output)
/* Load all tasks from caldav to "output"
 Implement an error system, emit..
*/
{
//    QSettings settings;
//	pCalendar->startSynchronization();
// #TODO optimize this !!!

	QList<CalendarEvent*>* events = new QList<CalendarEvent*>;

	pCalendar->allEvents(events);    

    for (auto i=events->begin();i!=events->end();i++)
    {
  	  task* t=new task((*i)->name(),QUuid::fromString((*i)->getUID()));
 	  output.push_back(t);
    }

}

void caldav::setMonitoring(bool b, QObject *parent)
/* */
{
    qDebug()<<"caldav::setFileWatch: "<<b<<endline;
	Q_UNUSED(parent);
}

QString caldav::getType()
/* return type of Backend*/
{
	return "caldav";
}




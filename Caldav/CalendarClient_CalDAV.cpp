/*********************************************//*!@addtogroup file Files*//*@{*/
/*!
 *******************************************************************************
* File identification:      $Id: CalendarClient_CalDAV.cpp 18 2017-01-26 17:33:06Z cypax $
* Revision of last commit:  $Rev: 18 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-01-26 18:33:06 +0100 (Do, 26 Jan 2017) $
 *******************************************************************************
 *
 * @file          CalendarClient_CalDAV.cpp
 * @author        Cypax (cypax.net)
 *
 * @brief         Implementation file of class CalendarClient_CalDAV.
 */
/************************************************************************//*@}*/

/******************************************************************************/
/* Library includes                                                           */
/******************************************************************************/
#include <QDebug>
#include <QColor>
#include <QList>

/******************************************************************************/
/* Own includes                                                               */
/******************************************************************************/
#include "CalendarClient_CalDAV.h"
#include "CalendarEvent.h"
#include "DateUtils.h"
#include "caldav_util.h"


/******************************************************************************/
/* Constants (#define)                                                        */
/***************************************//*!@addtogroup define Constants*//*@{*/

#define DEBUG_CALENDARCLIENT 1
#if DEBUG_CALENDARCLIENT
#define QDEBUG qDebug()
#else
#define QDEBUG if (0) qDebug()
#endif

/***** End of: define Constants *****************************************//*@}*/

/******************************************************************************/
/* Public functions                                                           */
/*****************************//*!@addtogroup pubfunct Public functions *//*@{*/



void CalendarClient_CalDAV::allEvents(QList<CalendarEvent*>* events_out)
{
  foreach(CalendarEvent* evt, this->m_EventList)
  {
    CalendarEvent* event = new CalendarEvent(this);
    event->copyFrom(*evt);
    events_out->append(event);
  }
}



int CalendarClient_CalDAV::getRequestTimeoutMS(void) const
{
  return m_RequestTimeoutMS;
}

void CalendarClient_CalDAV::setRequestTimeoutMS(const int requestTimeoutMS)
{
  if (requestTimeoutMS > 0)
  {
    m_RequestTimeoutMS = requestTimeoutMS;
    emit requestTimeoutMSChanged(m_RequestTimeoutMS);
  }
}


/***** End of: pubfunct Public functions ********************************//*@}*/

/******************************************************************************/
/* Protected functions                                                        */
/*************************//*!@addtogroup protfunct Protected functions *//*@{*/

QList<QObject*> CalendarClient_CalDAV::handleSingleEvent(CalendarEvent& evt, const QDateTime& startOfQuestionedDate, const QDateTime& endOfQuestionedDate)
{
  QList<QObject*> events;

  // events must not end at 00:00:00
  if (0 == evt.getEndDateTime().time().msecsSinceStartOfDay())
  {
    // removing one second from endDateTime
    QDateTime newEndDateTime = evt.getEndDateTime().addSecs(-1);
    evt.setEndDateTime(newEndDateTime);
  }

  // check if event occurs in questioned range
  if (    ( (evt.getStartDateTime() >= startOfQuestionedDate) && (evt.getStartDateTime() <= endOfQuestionedDate) ) // event starts in questioned range
       || ( (evt.getEndDateTime()   >= startOfQuestionedDate) && (evt.getEndDateTime()   <= endOfQuestionedDate) ) // event ends in questioned range
       || ( (evt.getStartDateTime() <= startOfQuestionedDate) && (evt.getEndDateTime()   >= endOfQuestionedDate) ) // event overlaps questioned range
     )
  {
    CalendarEvent* event = new CalendarEvent(this);
    event->copyFrom(evt);

    //QDEBUG << m_DisplayName << ": " << "appended" << event->name();
    events.append(event);
  }

  return events;
}

void CalendarClient_CalDAV::parseCALENDAR(QString href)
/* */
{
  QString line = m_DataStream->readLine();

  while(false == line.isNull())
  {
    if(false != line.contains("BEGIN:VEVENT"))
    {
        CalendarEvent* ce=new CalendarEvent(this);
        
        caldav_util::parseVEVENT(m_DataStream,ce);
		ce->setType(E_VEVENT);
		m_EventList.append(ce);
    }
    else if (false != line.contains("BEGIN:VTODO"))
    {
        CalendarEvent* ce=new CalendarEvent(this);
        
        caldav_util::parseVTODO(m_DataStream,ce);
		ce->setType(E_VTODO);
		m_EventList.append(ce);

    }
    line = m_DataStream->readLine();
  }
    
}


bool CalendarClient_CalDAV::setHostURL(const QUrl hostURL)
{
  bool bRet = false;

  if (false != hostURL.isValid())
  {
    m_HostURL = hostURL;
    bRet = true;
    emit hostURLChanged(m_HostURL.toString());
  }

  return bRet;
}


/******************************************************************************/
/* Constructors                                                               */
/**********************************//*!@addtogroup constrc Constructors *//*@{*/
CalendarClient_CalDAV::CalendarClient_CalDAV(QObject* parent)
{
	Q_UNUSED(parent);
	QDEBUG<<"CalendarClient_CalDAV::CalendarClient_CalDAV - initialising m_pRely";
  m_pReply = NULL;
  m_DataStream = NULL;
  m_HostURL = "";
  m_DisplayName = "";

  m_RequestTimeoutMS = 2000;
  m_RequestTimeoutTimer.setSingleShot(true);

  // timer is set to single shot so we don't need to abort it, in case the synchronization has failed
  m_SynchronizationTimer.setSingleShot(true);
  m_SynchronizationTimer.setInterval(600000);

  // create random color
  //m_Color = QColor(qrand() & 0xFF, qrand() & 0xFF, qrand() & 0xFF).name(); //GDE May2025
  m_Color="white";
  
  
  
//  m_CalendarType = E_CALENDAR_CALDAV;
  m_DataStream = NULL;
  m_pUploadReply = NULL;

  m_Username = "";
  m_Password = "";
  m_HostURL = "";
  m_syncToken = "";
  m_DisplayName = "";
  m_cTag = "";
  m_Year = 1;
  m_Month = 1;
  m_YearToBeRequested = QDate::currentDate().year();;
  m_MonthToBeRequested = QDate::currentDate().month();
  lastSyncYear = -1;
  lastSyncMonth = -1;
  m_bRecoveredFromError = false;
	//m_EventList = *new QList<CalendarEvent>;
  setupStateMachine();
}

/***** End of: constrc Constructors *************************************//*@}*/

/******************************************************************************/
/* Deconstructor                                                              */
/******************************//*!@addtogroup deconstrc Deconstructors *//*@{*/
CalendarClient_CalDAV::~CalendarClient_CalDAV()
{
  m_EventList.clear();
  m_SynchronizationTimer.stop();
}

/***** End of: deconstrc Constructors ***********************************//*@}*/

/******************************************************************************/
/* Public slots                                                               */
/*************************//*!@addtogroup pubslots Public slots         *//*@{*/


void CalendarClient_CalDAV::setUsername(const QString username)
{
  m_Username = username;
}

void CalendarClient_CalDAV::setPassword(const QString password)
{
  m_Password = password;
}

void CalendarClient_CalDAV::startSynchronization(void)
{
  QDEBUG << m_DisplayName << ": " << "!!!forcing synchronization!!!";
  emit forceSynchronization();
}


void CalendarClient_CalDAV::stopSynchronization(void)
{ //#TODO what should come here? ???
}

void CalendarClient_CalDAV::recover(void)
{
  QDEBUG << m_DisplayName << ": " << "trying to recover from EEROR state";
  m_bRecoveredFromError = true;
  emit recoverSignal();
}

void CalendarClient_CalDAV::setYear(const int& year)
{
  if (E_STATE_IDLE == m_State)
  {
    if (m_Year != year)
    {
      QDEBUG << m_DisplayName << ": " << "Year changed from" << m_Year << "to" << year;
      m_Year = year;
      emit yearChanged(m_Year);
      m_YearToBeRequested = year;
      startSynchronization();
    }
  }
  else
  {
    QDEBUG << m_DisplayName << ": " << "requested Year changed from" << m_YearToBeRequested << "to" << year;
    m_YearToBeRequested = year;
  }
}

void CalendarClient_CalDAV::setMonth(const int& month)
{
  if (E_STATE_IDLE == m_State)
  {
    if (m_Month != month)
    {
      QDEBUG << m_DisplayName << ": " << "Month changed from" << m_Month << "to" << month;
      m_Month = month;
      emit monthChanged(m_Month);
      m_MonthToBeRequested = month;
      startSynchronization();
    }
  }
  else
  {
    QDEBUG << m_DisplayName << ": " << "requested Month changed from" << m_MonthToBeRequested << "to" << month;
    m_MonthToBeRequested = month;
  }
}


void CalendarClient_CalDAV::saveEvent(CalendarEvent* evt)
{
  QDEBUG << m_DisplayName << ": " << "saving event" << evt->name();


  if (NULL != m_pUploadReply)
  {
    QDEBUG << m_DisplayName << ": " << "cleaning up m_pUploadReply";
    m_pUploadReply->abort();
    m_pUploadReply->disconnect();
  }

  QString authorization = "Basic ";
  authorization.append(caldav_util::encodeBase64(m_Username + ":" + m_Password));

  QBuffer* buffer = new QBuffer();

  buffer->open(QIODevice::ReadWrite);


  if (evt->getUID().isEmpty())
  {
    evt->setUID( QDateTime::currentDateTime().toString("yyyyMMdd-HHMM-00ss") + "-0000-" + evt->getStartDateTime().toString("yyyyMMddHHMM"));
  }

  QString filename = evt->getUID() + ".ics";

  QString requestString = "BEGIN:VCALENDAR\r\n";
  
	if(evt->getType()==E_VEVENT)
		requestString.append("BEGIN:VEVENT\r\n");
	else if (evt->getType()==E_VTODO)
		requestString.append("BEGIN:VTODO\r\n");

	requestString.append( "UID:" + evt->getUID() + "\r\n"
                          "VERSION:2.0\r\n"
                          "DTSTAMP:" + QDateTime::currentDateTime().toString("yyyyMMddTHHmmssZ") + "\r\n"
                          "SUMMARY:" + evt->name() + "\r\n"
                          //"DTSTART:" + (evt->getStartDateTime()).toString("yyyyMMddTHHmmss") + "\r\n"
                          //"DTEND:" + (evt->getEndDateTime()).toString("yyyyMMddTHHmmss") + "\r\n"
                          //"LOCATION:" + evt->location() + "\r\n"
                          "DESCRIPTION:" + evt->description() + "\r\n"
                          "TRANSP:OPAQUE\r\n");

  if (!evt->getRRULE().isEmpty())
	    requestString.append("RRULE:" + evt->getRRULE() + "\r\n");

  if (!evt->getExdates().isEmpty())
	    requestString.append("EXDATE:" + evt->getExdates() + "\r\n");

  if(evt->getType()==E_VEVENT)
		requestString.append("END:VEVENT\r\nEND:VCALENDAR");
  else if (evt->getType()==E_VTODO)
		requestString.append("END:VTODO\r\nEND:VCALENDAR");


QDEBUG<<"BEFORE UPLOAD: request="<<requestString;

  int buffersize = buffer->write(requestString.toUtf8());
  buffer->seek(0);
  buffer->size();

  QByteArray contentlength;
  contentlength.append(QByteArray::number(buffersize)); //GDE may2025

  QNetworkRequest request;
  request.setUrl(QUrl(m_HostURL.toString() + filename));
  request.setRawHeader("User-Agent", "CalendarClient_CalDAV");
  request.setRawHeader("Authorization", authorization.toUtf8());
  request.setRawHeader("Depth", "0");
  request.setRawHeader("Prefer", "return-minimal");
  request.setRawHeader("Content-Type", "text/calendar; charset=utf-8");
  request.setRawHeader("Content-Length", contentlength);


  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  m_pUploadReply = m_UploadNetworkManager.put(request, buffer);

  if (NULL != m_pUploadReply)
  {
    connect(m_pUploadReply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)),
            this, SLOT(handleUploadHTTPError()));

    connect(m_pUploadReply, SIGNAL(finished()),
            this, SLOT(handleUploadFinished()));

    m_UploadRequestTimeoutTimer.start(m_RequestTimeoutMS);
  }
  else
  {
    QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when requesting URL.";
    emit error("Invalid reply pointer when requesting URL.");
  }

}


void CalendarClient_CalDAV::deleteEvent(QString href)
{
  if (href.isEmpty())
  {
    return;
  }

  QDEBUG << m_DisplayName << ": " << "deleting event with HREF" << href;

  if (NULL != m_pUploadReply)
  {
    QDEBUG << m_DisplayName << ": " << "cleaning up m_pUploadReply";
    m_pUploadReply->abort();
    m_pUploadReply->disconnect();
  }

  QString authorization = "Basic ";
  authorization.append(caldav_util::caldav_util::encodeBase64(m_Username + ":" + m_Password));

  QString filename = QUrl(href).fileName();

  QNetworkRequest request;
  request.setUrl(QUrl(m_HostURL.toString() + filename));
  request.setRawHeader("User-Agent", "CalendarClient_CalDAV");
  request.setRawHeader("Authorization", authorization.toUtf8());
  request.setRawHeader("Depth", "0");
  request.setRawHeader("Prefer", "return-minimal");
  request.setRawHeader("Content-Type", "text/calendar; charset=utf-8");
  request.setRawHeader("Content-Length", 0);


  QDEBUG << m_DisplayName << ": " << "deleting" << request.url();


  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  m_pUploadReply = m_UploadNetworkManager.deleteResource(request);

  if (NULL != m_pUploadReply)
  {
    connect(m_pUploadReply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)),
            this, SLOT(handleUploadHTTPError()));

    connect(m_pUploadReply, SIGNAL(finished()),
            this, SLOT(handleUploadFinished()));

    m_UploadRequestTimeoutTimer.start(m_RequestTimeoutMS);
  }
  else
  {
    QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when requesting URL.";
    emit error("Invalid reply pointer when requesting URL.");
  }

}


void CalendarClient_CalDAV::handleUploadHTTPError(void)
{
  m_UploadRequestTimeoutTimer.stop();
  if (NULL != m_pUploadReply)
  {
    QDEBUG << m_DisplayName << ": " << "HTTP upload error:" << m_pUploadReply->errorString();
    emit error(m_pUploadReply->errorString());
  }
  else
  {
    QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when handling HTTP error.";
    emit error("Invalid reply pointer when handling HTTP error.");
  }

}

void CalendarClient_CalDAV::handleUploadFinished(void)
{
  m_UploadRequestTimeoutTimer.stop();

  QDEBUG << m_DisplayName << ": " << "HTTP upload finished";

  if (NULL != m_pUploadReply)
  {
    QDEBUG << m_DisplayName << ": " << "received:\r\n" << m_pUploadReply->readAll();
    emit forceSynchronization();
  }
}




#include <QDomDocument>


void CalendarClient_CalDAV::handleHTTPError(void)
{
  m_State = E_STATE_ERROR;
  emit syncStateChanged(m_State);
  m_RequestTimeoutTimer.stop();
  if (NULL != m_pReply)
  {
    QDEBUG << m_DisplayName << ": " << "HTTP request error:" << m_pReply->errorString();
    emit error(m_pReply->errorString());
  }
  else
  {
    QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when handling HTTP error.";
    emit error("Invalid reply pointer when handling HTTP error.");
  }
}

void CalendarClient_CalDAV::handleRequestSyncTokenFinished(void)
{
  m_RequestTimeoutTimer.stop();

  if (E_STATE_ERROR == m_State)
  {
    QDEBUG << m_DisplayName << ": " << "Error state - aborting";
  }

  QDEBUG << m_DisplayName << ": " << "HTTP RequestSyncToken finished";

  if (NULL != m_pReply)
  {
    QDomDocument doc;

    doc.setContent(m_pReply);
//	QDEBUG<<doc.toByteArray();
    QString sDisplayName = "";
    QString sCTag = "";
    QString sSyncToken = "";
    QString sStatus = "";


    QDomNodeList response = doc.elementsByTagName("prop");
    for (int i = 0; i < response.size(); i++)
    {
    	QDEBUG << "   "<<response.item(i).nodeValue();
        QDomNode n = response.item(i);
        QDomElement displayname = n.firstChildElement("displayname");
        if (!displayname.isNull())
        {
          QDEBUG << m_DisplayName << ": " << "DISPLAYNAME = " << displayname.text();
          sDisplayName = displayname.text();
        }
        QDomElement ctag = n.firstChildElement("cs:getctag");
        if (!ctag.isNull())
        {
          QDEBUG << m_DisplayName << ": " << "CTAG = " << ctag.text();
          sCTag = ctag.text();
        }
        QDomElement syncToken = n.firstChildElement("sync-token");
        if (!syncToken.isNull())
        {
          QDEBUG << m_DisplayName << ": " << "SYNC-TOKEN = " << syncToken.text();
          sSyncToken = syncToken.text();
        }
    }

    response = doc.elementsByTagName("propstat");
    for (int i = 0; i < response.size(); i++)
    {
        QDomNode n = response.item(i);
        QDomElement status = n.firstChildElement("status");
        if (!status.isNull())
        {
          QDEBUG << m_DisplayName << ": " << "STATUS = " << status.text();
          sStatus = status.text();
        }
    }
	
    if ( (!sSyncToken.isEmpty()) && (sStatus.endsWith("200 OK")) )
    {
      bool bDisplayNameChanged = (m_DisplayName != sDisplayName);
      bool bSyncTokenChanged = (m_syncToken != sSyncToken);

      if (true == bSyncTokenChanged)
      {
        QDEBUG << m_DisplayName << ": " << "sync token has changed from"  << m_syncToken << "to" << sSyncToken;
      }

      m_DisplayName = sDisplayName;
      m_cTag = sCTag;
      m_syncToken = sSyncToken;

      if (false != bDisplayNameChanged)
      {
        emit displayNameChanged(m_DisplayName);
      }

      if (false == bSyncTokenChanged)
      {
        QDEBUG << m_DisplayName << ": " << "sync token is unchanged";
        emit syncTokenHasNotChanged();

        if ( (m_Year != lastSyncYear) || (m_Month != lastSyncMonth) )
        {
          QDEBUG << m_DisplayName << ": " << "year/month has changed from" << lastSyncYear << lastSyncMonth << "to" << m_Year << m_Month << "=> update required";
          lastSyncMonth = m_Month;
          lastSyncYear = m_Year;
          emit calendarUpdateRequired();
        }
        else
        {
          QDEBUG << m_DisplayName << ": " << "calendar has not changed, no update required";
          m_SynchronizationTimer.start();
          emit calendarHasNotChanged();
        }
      }
      else
      {
        QDEBUG << m_DisplayName << ": " << "sync token has changed";
        emit syncTokenChanged();
        lastSyncMonth = m_Month;
        lastSyncYear = m_Year;
        emit calendarUpdateRequired();
      }
    }
    else
    {
      QDEBUG << m_DisplayName << ": " << "ERROR: Receiving sync token failed. Status:" << sStatus;
      emit error("Receiving sync token failed.");
    }

    /*
    QDEBUG << m_DisplayName << ": " << "\r\nHeaders:" << m_pReply->rawHeaderList() << "\r\n";

    if (m_pReply->hasRawHeader("DAV")) {
      QDEBUG << m_DisplayName << ": " << "DAV:" << m_pReply->rawHeader("DAV");
    }
    if (m_pReply->hasRawHeader("Server")) {
      QDEBUG << m_DisplayName << ": " << "Server:" << m_pReply->rawHeader("Server");
    }
    QDEBUG << m_DisplayName << ": " << "Status code:" << m_pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    */
  }
  else
  {
    QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when receiving sync token.";
    emit error("Invalid reply pointer when receiving sync token.");
  }

}


void CalendarClient_CalDAV::handleRequestChangesFinished(void)
{
  m_RequestTimeoutTimer.stop();

  if (E_STATE_ERROR == m_State)
  {
    QDEBUG << m_DisplayName << ": " << "Error state - aborting";
  }

  QDEBUG << m_DisplayName << ": " << "HTTP RequestChanges finished";

  if (NULL == m_pReply)
  {
    QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when receiving changes.";
    emit error("Invalid reply pointer when receiving changes.");
  }
  else
  {
    QDomDocument doc;

    //QDEBUG << m_DisplayName << ": " << "received:\r\n" << m_pReply->readAll();

    doc.setContent(m_pReply);

    m_EventList.clear();

    QDomNodeList list_response = doc.elementsByTagName("response");
    for (int i = 0; i < list_response.size(); i++)
    {
      QDomNode thisResponse = list_response.item(i);

      //QDEBUG << m_DisplayName << ": " << "Response" << i;

      QString sHref = "";
      QString sETag = "";
      QString sPropStatus = "";
      QString strCalendarData = "";

      QDomElement elHref = thisResponse.firstChildElement("href");
      if (!elHref.isNull())
      {
        //QDEBUG << m_DisplayName << ": " << "  HREF = " << elHref.text();
        sHref = elHref.text();
      }
      else
      {
        QDEBUG << m_DisplayName << ": " << "  HREF = ";
      }


      QDomNode thisPropStat = thisResponse.firstChildElement("propstat");
      if (!thisPropStat.isNull())
      {

        QDomElement elPropStatus = thisPropStat.firstChildElement("status");
        if (!elPropStatus.isNull())
        {
          //QDEBUG << m_DisplayName << ": " << "    PROPSTATUS = " << elPropStatus.text();
          sPropStatus = elPropStatus.text();
        }
        else
        {
          QDEBUG << m_DisplayName << ": " << "    PROPSTATUS = ";
        }


        QDomNode thisProp = thisPropStat.firstChildElement("prop");
        if (!thisProp.isNull())
        {
          QDomElement elETag = thisProp.firstChildElement("getetag");
          if (!elETag.isNull())
          {
            //QDEBUG << m_DisplayName << ": " << "    ETAG = " << elETag.text();
            sETag = elETag.text();
          }
          else
          {
            QDEBUG << m_DisplayName << ": " << "    ETAG = ";
          }

          QDomElement elCalendarData = thisProp.firstChildElement("C:calendar-data");
          if (!elCalendarData.isNull())
          {
            QDEBUG << m_DisplayName << ": " << "    CALENDARDATA = " << elCalendarData.text();

            if (m_DataStream)
	              delete m_DataStream;

            m_DataStream = new QTextStream(elCalendarData.text().toLatin1());

            parseCALENDAR(sHref);

            strCalendarData = elCalendarData.text();
          }
          else
          {
            QDEBUG << m_DisplayName << ": " << "    CALENDARDATA = ";
          }

        }
      }
    }

    int iStatusCode = m_pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QDEBUG << m_DisplayName << ": " << "Status code:" << iStatusCode;

    if (207 != iStatusCode)
    {
      QDEBUG << m_DisplayName << ": " << "ERROR: Invalid HTTP return code:" << iStatusCode;
      emit error("Invalid HTTP return code.");
    }
    else
    {
      m_SynchronizationTimer.start();
      emit eventsUpdated();
    }

  }

}




/******************************************************************************/
/* Protected functions                                                        */
/*************************//*!@addtogroup protfunct Protected functions *//*@{*/


void CalendarClient_CalDAV::sendRequestSyncToken(void)
{
  if (NULL != m_pReply)
  {
    QDEBUG << m_DisplayName << ": " << "cleaning up m_pReply";
    m_pReply->abort();
    m_pReply->disconnect();
  }

  QString authorization = "Basic ";
  authorization.append(caldav_util::encodeBase64(m_Username + ":" + m_Password));

  QBuffer* buffer = new QBuffer();

  buffer->open(QIODevice::ReadWrite);



  QString requestString = "<d:propfind xmlns:d=\"DAV:\" xmlns:cs=\"http://calendarserver.org/ns/\">\r\n"
                          "  <d:prop>\r\n"
                          "    <d:displayname />\r\n"
                          "    <cs:getctag />\r\n"
                          "    <d:sync-token />"
                          "  </d:prop>\r\n"
                          "</d:propfind>";

  int buffersize = buffer->write(requestString.toUtf8());
  buffer->seek(0);
  buffer->size();

  QByteArray contentlength;
  contentlength.append(QByteArray::number(buffersize)); //GDE May2025

  QNetworkRequest request;
  request.setUrl(m_HostURL);
  request.setRawHeader("User-Agent", "CalendarClient_CalDAV");
  request.setRawHeader("Authorization", authorization.toUtf8());
  request.setRawHeader("Depth", "0");
  request.setRawHeader("Prefer", "return-minimal");
  request.setRawHeader("Content-Type", "text/xml; charset=utf-8");
  request.setRawHeader("Content-Length", contentlength);


	QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);
//  request.setSslConfiguration(request.sslConfiguration().setPeerVerifyMode(QSslSocket::VerifyNone));

  m_pReply = m_NetworkManager.sendCustomRequest(request, QByteArray("PROPFIND"), buffer);

  if (NULL != m_pReply)
  {
    connect(m_pReply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)),
            this, SLOT(handleHTTPError()));

    connect(m_pReply, SIGNAL(finished()),
            this, SLOT(handleRequestSyncTokenFinished()));

    m_RequestTimeoutTimer.start(m_RequestTimeoutMS);
  }
  else
  {
    QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when requesting sync token.";
    emit error("Invalid reply pointer when requesting sync token.");
  }
  
 
}



void CalendarClient_CalDAV::sendRequestChanges(void)
{
  QString authorization = "Basic ";
  authorization.append(caldav_util::encodeBase64(m_Username + ":" + m_Password));

  QBuffer* buffer = new QBuffer();

  buffer->open(QIODevice::ReadWrite);

  QString monthString = QString::number(m_Month);
  if (monthString.length() < 2) monthString.prepend("0");

  QString lastDayString = QString::number(DateUtils::lastDayOfMonth(m_Year, m_Month));
  if (lastDayString.length() < 2) lastDayString.prepend("0");


  QString requestString = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\r\n"
                          "<C:calendar-query xmlns:D=\"DAV:\" xmlns:C=\"urn:ietf:params:xml:ns:caldav\">\r\n"
                          " <D:prop>\r\n"
                          "   <D:getetag/>\r\n"
                          "   <C:calendar-data>\r\n"
                          "     <C:comp name=\"VCALENDAR\">\r\n"
                          "       <C:prop name=\"VERSION\"/>\r\n"
                          "       <C:comp name=\"VTODO\">\r\n"               //GDE  EVENT->TASK
                          "         <C:prop name=\"SUMMARY\"/>\r\n"
                          "         <C:prop name=\"LOCATION\"/>\r\n"
                          "         <C:prop name=\"DESCRIPTION\"/>\r\n"
                          "         <C:prop name=\"UID\"/>\r\n"
                          "         <C:prop name=\"DTSTART\"/>\r\n"
                          "         <C:prop name=\"DTEND\"/>\r\n"
                          "         <C:prop name=\"LAST-MODIFIED\"/>\n"
                          "		    <C:prop name=\"PARTSTAT\"/>\r\n"
                          "       </C:comp>\r\n"
                          "     </C:comp>\r\n"
                          "   </C:calendar-data>\r\n"
                          " </D:prop>\r\n"
                          " <C:filter>\r\n"
                          "   <C:comp-filter name=\"VCALENDAR\">\r\n"
                          "     <C:comp-filter name=\"VTODO\">\r\n"               //GDE  EVENT->TASK
                          //"       <C:time-range start=\"" + QString::number(m_Year) + monthString + "01T000000Z\" end=\"" + QString::number(m_Year) + monthString + lastDayString + "T235959Z\"/>\r\n"                    //GDE  EVENT->TASK
                          "     </C:comp-filter>\r\n"
                          "   </C:comp-filter>\r\n"
                          " </C:filter>\r\n"
                          "</C:calendar-query>\r\n";

  //QDEBUG << m_DisplayName << ": " << "requesting:";
  //QDEBUG << m_DisplayName << ": " << requestString;
  //QDEBUG << m_DisplayName << ": " << "<C:time-range start=\"" + QString::number(m_Year) + monthString + "01T000000Z\" end=\"" + QString::number(m_Year) + monthString + lastDayString + "T235959Z\"/>";

  int buffersize = buffer->write(requestString.toUtf8());
  buffer->seek(0);
  buffer->size();

  QByteArray contentlength;
  contentlength.append(QByteArray::number(buffersize)); //GDE May2025

  QNetworkRequest request;
  request.setUrl(m_HostURL);
  request.setRawHeader("User-Agent", "CalendarClient_CalDAV");
  request.setRawHeader("Authorization", authorization.toUtf8());
  request.setRawHeader("Depth", "1");
  request.setRawHeader("Content-Type", "application/xml; charset=utf-8");
  request.setRawHeader("Content-Length", contentlength);

  QSslConfiguration conf = request.sslConfiguration();
  conf.setPeerVerifyMode(QSslSocket::VerifyNone);
  request.setSslConfiguration(conf);

  m_pReply = m_NetworkManager.sendCustomRequest(request, QByteArray("REPORT"), buffer);

  if (NULL != m_pReply)
  {
    connect(m_pReply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)),
            this, SLOT(handleHTTPError()));

    connect(m_pReply, SIGNAL(finished()),
            this, SLOT(handleRequestChangesFinished()));

    m_RequestTimeoutTimer.start(m_RequestTimeoutMS);
  }
  else
  {
    QDEBUG << m_DisplayName << ": " << "ERROR: Invalid reply pointer when requesting sync token.";
    emit error("Invalid reply pointer when requesting sync token.");
  }
}
/**** Last line of source code                                             ****/

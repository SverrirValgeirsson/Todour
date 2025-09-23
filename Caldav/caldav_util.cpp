#include "caldav_util.h"
#include "DateUtils.h"
#include <QByteArray>

#define DEBUG_CALDAVUTIL 0
#if DEBUG_CALDAVUTIL
#define QDEBUG qDebug()
#else
#define QDEBUG if (0) qDebug()
#endif




 QString caldav_util::ascii2utf8(QString str)
{
 return QString::fromUtf8(str.toLocal8Bit().data());
}


QString caldav_util::encodeBase64(QString string)
{
  QByteArray ba;
  ba.append(string.toUtf8());
  return ba.toBase64();
}

void caldav_util::parseVTODO( QTextStream* m_DataStream, CalendarEvent* event)
/* */
{
//  event.setColor(m_Color);
//  event.setCalendarName(m_DisplayName);
//  event.setCalendarPointer(this);
//  event->setHREF(href);
 QString line;
  QDateTime utcTime;
  while (!(line = m_DataStream->readLine()).contains(QByteArray("END:VTODO")))
  {
    //QDEBUG << m_DisplayName << ": " << line;

    const int deliminatorPosition = line.indexOf(QLatin1Char(':'));
    const QString key   = line.mid(0, deliminatorPosition);
    QString value = (line.mid(deliminatorPosition + 1, -1).replace("\\n", "\n")); //.toLatin1();
    QString testEncodingString = ascii2utf8(value);
    if (false == testEncodingString.contains("�"))
    {
      value = testEncodingString;
    }

    if (key.startsWith(QLatin1String("DTSTAMP")))
    {
      utcTime = DateUtils::parseDate(value);
      event->setStartDateTime(utcTime.toLocalTime());
    }
    else if (key.startsWith(QLatin1String("DTSTAMP")))
    {
      utcTime = DateUtils::parseDate(value);
      event->setEndDateTime(utcTime.toLocalTime());
    }
    else if (key == QLatin1String("RRULE"))
    {
      event->setRRULE(value);
    }
    else if (key == QLatin1String("EXDATE"))
    {
      event->setExdates(value);
    }
    else if (key == QLatin1String("SUMMARY"))
    {
      event->setName(value);
    }
    else if (key == QLatin1String("LOCATION"))
    {
      event->setLocation(value);
    }
    else if (key == QLatin1String("UID"))
    {
      event->setUID(value);
    }
    else if (key == QLatin1String("CATEGORIES"))
    {
      event->setCategories(value);
    }
    else if (key == QLatin1String("DESCRIPTION"))
    {
      event->setDescription(value);
    }
    else if (key == QLatin1String("LAST-MODIFIED"))
    {
      utcTime = DateUtils::parseDate(value);
      event->setLastModified(utcTime.toLocalTime());
    }

  }


}




 void caldav_util::parseVEVENT(QTextStream* m_DataStream, CalendarEvent* event)
{
//  event.setColor(m_Color);
//  event.setCalendarName(m_DisplayName);
//  event.setCalendarPointer(this);
//  event->setHREF(href);
  QString line;
  QDateTime utcTime;
  while (!(line = m_DataStream->readLine()).contains(QByteArray("END:VEVENT")))
  {
    //QDEBUG << m_DisplayName << ": " << line;

    const int deliminatorPosition = line.indexOf(QLatin1Char(':'));
    const QString key   = line.mid(0, deliminatorPosition);
    QString value = (line.mid(deliminatorPosition + 1, -1).replace("\\n", "\n")); //.toLatin1();
    QString testEncodingString = ascii2utf8(value);
    if (false == testEncodingString.contains("�"))
    {
      value = testEncodingString;
    }

    if (key.startsWith(QLatin1String("DTSTART")))
    {
      utcTime = DateUtils::parseDate(value);
      event->setStartDateTime(utcTime.toLocalTime());
    }
    else if (key.startsWith(QLatin1String("DTEND")))
    {
      utcTime = DateUtils::parseDate(value);
      event->setEndDateTime(utcTime.toLocalTime());
    }
    else if (key == QLatin1String("RRULE"))
    {
      event->setRRULE(value);
    }
    else if (key == QLatin1String("EXDATE"))
    {
      event->setExdates(value);
    }
    else if (key == QLatin1String("SUMMARY"))
    {
      event->setName(value);
    }
    else if (key == QLatin1String("LOCATION"))
    {
      event->setLocation(value);
    }
    else if (key == QLatin1String("UID"))
    {
      event->setUID(value);
    }
    else if (key == QLatin1String("CATEGORIES"))
    {
      event->setCategories(value);
    }
    else if (key == QLatin1String("DESCRIPTION"))
    {
      event->setDescription(value);
    }
  }
  
}



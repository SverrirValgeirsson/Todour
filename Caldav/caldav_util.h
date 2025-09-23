// moved by GDE to a new files, static functions to simplify the code.
// GPL

#include <QDateTime>
#include <QTextStream>


#include "CalendarEvent.h"

#ifndef CALDAV_UTIL_H
#define CALDAV_UTIL_H

class caldav_util
{
public:

static void parseVTODO( QTextStream* m_DataStream, CalendarEvent* event);


static void parseVEVENT(QTextStream* m_dataStream, CalendarEvent* event);
//static void parseRRULE(CalendarEvent* evt);

 /**
   * @brief Helper function to decode received strings.
   */
static QString ascii2utf8(QString str);

  /**
   * @brief Helper function to encode network authorization requests.
   */
static QString encodeBase64(QString string);

};

#endif  //CALDAV_UTIL_H

/*********************************************//*!@addtogroup file Files*//*@{*/
/*!
 *******************************************************************************
* File identification:      $Id: CalendarEvent.h 11 2017-01-16 20:27:13Z cypax $
* Revision of last commit:  $Rev: 11 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-01-16 21:27:13 +0100 (Mo, 16 Jan 2017) $
 *******************************************************************************
 *
 * @author        Cypax (cypax.net)
 *
 * @brief         Class for Calendar events.
 */
/************************************************************************//*@}*/

#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include <QDateTime>
#include <QObject>
#include <QString>

/******************************************************************************/
/* Constants (#define)                                                        */
/***************************************//*!@addtogroup define Constants*//*@{*/
  typedef enum {
    E_INVALID,	//item not defined. won't be saved!
    E_VEVENT,  // item is a calendar event
    E_VTODO,  // item is a todo event
    E_VJOURNAL //item is a journal event (see rfc5545)
  } E_CalType;


/******************************************************************************/
/* Class                                                                      */
/******************************************//*!@addtogroup class Classes*//*@{*/
/**
* @brief CalendarEvent represents an event within an ICS or CalDAV calendar.
*
* When owned by a CalendarClient object, the start date and end date of the
* CalendarEvent object are stored as specified by the CalDAV server or ICS file.
* Thus the event might apply to several dates if a RRULE value is set.
**/
class CalendarEvent : public QObject
{
  Q_OBJECT

/******************************************************************************/
/* Types (typedef)                                                            */
/******************************************//*!@addtogroup typedef Types*//*@{*/
public:
  Q_ENUMS(E_CalType)

/***** End of: typedef Types ********************************************//*@}*/

/******************************************************************************/
/* Properties (Q_PROPERTY)                                                    */
/**********************************//*!@addtogroup Q_PROPERTY Properties*//*@{*/

  // Display name of the calendar where this event originates from
  Q_PROPERTY(QString calendarName READ calendarName WRITE setCalendarName NOTIFY calendarNameChanged)

  // Color of the calendar where this event originates from
  Q_PROPERTY(QString color READ getColor WRITE setColor NOTIFY colorChanged)

  // Summary or title of the event
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

  // Location, where the event occurs
  Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)

  // Detailed description of the event
  Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

  // Start date and time; event applies since this point in time
  Q_PROPERTY(QDateTime startDateTime READ getStartDateTime WRITE setStartDateTime NOTIFY startDateTimeChanged)

  // End date and time; event applies until this point in time
  Q_PROPERTY(QDateTime endDateTime READ getEndDateTime WRITE setEndDateTime NOTIFY endDateTimeChanged)

  // iCalendar RRULE value as string
  Q_PROPERTY(QString rrule READ getRRULE WRITE setRRULE NOTIFY rruleChanged)

  // iCalendar EXDATE value as string
  Q_PROPERTY(QString exdates READ getExdates WRITE setExdates NOTIFY exdatesChanged)

  // Event categories, separated by ","
  Q_PROPERTY(QString categories READ getCategories WRITE setCategories NOTIFY categoriesChanged)

  // Unique ID of event
  Q_PROPERTY(QString uid READ getUID WRITE setUID NOTIFY uidChanged)

  // URL of calendar file
  Q_PROPERTY(QString href READ getHREF WRITE setHREF NOTIFY hrefChanged)
  
  // Last modified
  Q_PROPERTY(QDateTime lastmodified READ getLastModified WRITE setLastModified NOTIFY lastModifiedChanged)

  // Read-only boolean property which is set, if the event represents a specific occurrence and an EXDATE value applies
  Q_PROPERTY(bool isCanceled READ getIsCanceled NOTIFY isCanceledChanged)

  // Type of event
  Q_PROPERTY(E_CalType eventType READ getType WRITE setType)

/***** End of: Q_PROPERTY Properties ************************************//*@}*/

/******************************************************************************/
/* Constructors                                                               */
/******************************************************************************/
public:
  explicit CalendarEvent(QObject* parent);
  CalendarEvent(const CalendarEvent& other);

/******************************************************************************/
/* Public function prototypes                                                 */
/******************************************************************************/
public:
  CalendarEvent& operator=(const CalendarEvent& other);

  bool operator<(const CalendarEvent& other) const;

  void copyFrom(const CalendarEvent& other);

  void reset();
	
/******************************************************************************/
/* Protected function prototypes                                                */
/******************************************************************************/
protected:

/******************************************************************************/
/* Private function prototypes                                                */
/******************************************************************************/
private:

/******************************************************************************/
/* Signals                                                                    */
/******************************************************************************/
signals:
  void colorChanged(QString& color);
  void calendarNameChanged(const QString& calendarName);
  void nameChanged(const QString &name);
  void locationChanged(const QString& location);
  void descriptionChanged(const QString& description);
  void startDateTimeChanged(const QDateTime &startDateTime);
  void endDateTimeChanged(const QDateTime &endDateTime);
  void rruleChanged(const QString& rrule);
  void exdatesChanged(const QString& exdates);
  void categoriesChanged(const QString& categories);
  void uidChanged(const QString& uid);
  void hrefChanged(const QString& href);
  void isCanceledChanged(const bool& isCanceled);
  void lastModifiedChanged(const QDateTime& lastModified);

/******************************************************************************/
/* Public slots                                                               */
/******************************************************************************/
public slots:
  inline QString getColor(void) const{  return m_Color;};
  void setColor(const QString& color);

  inline QString calendarName() const{  return m_calendarName;};
  void setCalendarName(const QString &calendarName);

  inline QString name() const{  return m_Name;};
  void setName(const QString &name);

  inline QString location() const{  return m_Location;};
  void setLocation(const QString& location);

  inline QString description() const{  return m_Description;};
  void setDescription(const QString& description);

  inline QDateTime getStartDateTime() const{    return m_StartDateTime;};
  void setStartDateTime(const QDateTime &startDateTime);

  inline QDateTime getEndDateTime(void) const{  return m_EndDateTime;};
  void setEndDateTime(const QDateTime &endDateTime);

  inline QString getRRULE() const{return m_RRULE;};
  void setRRULE(const QString& rRule);

  inline QString getExdates() const{  return m_Exdates;};
  void setExdates(const QString& exdates);

  inline QString getCategories() const{  return m_Categories;};
  void setCategories(const QString& categories);

  inline bool getIsCanceled() const{return m_IsCanceled;};
  void setIsCanceled(const bool& isCanceled);

  inline QString getUID(void) const{return m_UID;};
  void setUID(const QString& uid);

  inline QString getHREF(void) const{  return m_HREF;};
  void setHREF(const QString& href);

  inline E_CalType getType() const {return eventType;};
  inline void setType(E_CalType _et){eventType=_et;};

  inline QDateTime getLastModified() const{    return m_lastModified;};
  void setLastModified(const QDateTime &date);


  // to edit an event and upload it back to the CalDAV server we need to know the
  // CalendarClient object where this event originates from
  inline QObject* getCalendarPointer(void) const{  return m_CalendarPointer;};
  inline void setCalendarPointer(QObject* ptr){m_CalendarPointer = ptr;};

/******************************************************************************/
/* Protected slots                                                            */
/******************************************************************************/
protected slots:

/******************************************************************************/
/* Private slots                                                               */
/******************************************************************************/
private slots:

/******************************************************************************/
/* Protected attributes                                                         */
/******************************************************************************/
protected:
  QString m_Color;
  QString m_calendarName;
  QString m_Name;
  QString m_Location;
  QString m_Description;
  QDateTime m_StartDateTime;
  QDateTime m_EndDateTime;
  QString m_RRULE;
  QString m_Exdates;
  QString m_Categories;
  QString m_UID;
  QString m_HREF;
  bool m_IsCanceled;
  QObject* m_CalendarPointer;
  E_CalType eventType;
  QDateTime m_lastModified;


/******************************************************************************/
/* Private attributes                                                         */
/******************************************************************************/
private:

};

/***** End of: Classes **************************************************//*@}*/

#endif // CALENDAREVENT_H
/**** Last line of source code                                             ****/

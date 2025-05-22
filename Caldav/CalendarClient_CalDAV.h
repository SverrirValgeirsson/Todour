/*********************************************//*!@addtogroup file Files*//*@{*/
/*!
 *******************************************************************************
* File identification:      $Id: CalendarClient_CalDAV.h 17 2017-01-26 17:25:48Z cypax $
* Revision of last commit:  $Rev: 17 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-01-26 18:25:48 +0100 (Do, 26 Jan 2017) $
 *******************************************************************************
 *
 * @file          CalendarClient_CalDAV.h
 * @author        Cypax (cypax.net)
 *
 * @brief         CalDAV based calendar client.
 *
 * This represent a connection to a CalDAV server.
 * We can connect, disconnect
 * read data
 * (future) read data with condition
 * send (write) data
 * get connection state
 * 
 */
/************************************************************************//*@}*/

#ifndef CALENDARCLIENT_CALDAV_H
#define CALENDARCLIENT_CALDAV_H

/******************************************************************************/
/* Library includes                                                           */
/******************************************************************************/
#include <QObject>

#include <QTimer>
#include <QList>
#include <QDate>
#include <QUrl>
#include <QTextStream>
#include <QtNetwork>
/******************************************************************************/
/* Own includes                                                               */
/******************************************************************************/
#include "CalendarEvent.h"

/******************************************************************************/
/* Class                                                                      */
/******************************************//*!@addtogroup class Classes*//*@{*/
/**
* @brief Calendar class which obtains CalendarEvent objects from a CalDAV server.
*
* The CalendarClient_CalDAV class works with owncloud, nextcloud and possibly
* with other calDAv based servers.
* The URL property can be in the style
* https://server.tld/owncloud/remote.php/dav/calendars/username/calendarname/
* It is highly recommended to use HTTPS to protect your personal data.
*
* @note CalendarClient_CalDAV uses the year and month properties to limit the
* list of events to a specific month.
**/
class CalendarClient_CalDAV : public QObject
{
  Q_OBJECT

/******************************************************************************/
/* Types (typedef)                                                            */
/******************************************//*!@addtogroup typedef Types*//*@{*/
public:
  Q_ENUMS(E_CalendarState)
//***  Q_ENUMS(E_CalendarType)

/******************************************************************************/
/* Types (typedef)                                                            */
/******************************************//*!@addtogroup typedef Types*//*@{*/
public:

  typedef enum {
    E_STATE_IDLE,  // calendar is waiting for next synchronization
    E_STATE_BUSY,  // calendar is currently synchronizing
    E_STATE_ERROR  // calendar has encountered an error condition
  } E_CalendarState;

/***  typedef enum {
    E_CALENDAR_ICS,     // event source is a local or remote iCalendar file
    E_CALENDAR_CALDAV   // event source is a CalDAV server
  } E_CalendarType;
*/

/***** End of: typedef Types ********************************************//*@}*/

/******************************************************************************/
/* Properties (Q_PROPERTY)                                                    */
/**********************************//*!@addtogroup Q_PROPERTY Properties*//*@{*/
  // Assinged color of this CalendarClient for graphical illustration in GUIs
//***  Q_PROPERTY(QString color READ getColor WRITE setColor NOTIFY colorChanged)

  // Synchronization state of the CalendarClient
  Q_PROPERTY(E_CalendarState syncState READ getSyncState NOTIFY syncStateChanged)

  // Type of calendar; i.e. what the source of calendar events is
//***  Q_PROPERTY(E_CalendarType calendarType READ getCalendarType CONSTANT)

  // URL of calendar events source (iCalendar file path, CalDAV server URL, ...)
  Q_PROPERTY(QUrl hostURL READ getHostURL WRITE setHostURL NOTIFY hostURLChanged)

  // Displayed name of the CalendarClient instance; might be overwritten by received calendar information
//  Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)

  // Request timeout in ms for accessing remote calendar sources (default: 2000ms)
  Q_PROPERTY(int    requestTimeoutMS
             READ   getRequestTimeoutMS
             WRITE  setRequestTimeoutMS
             NOTIFY requestTimeoutMSChanged)


  Q_PROPERTY(int year READ getYear WRITE setYear NOTIFY yearChanged)
  Q_PROPERTY(int month READ getMonth WRITE setMonth NOTIFY monthChanged)
  Q_PROPERTY(QString  username  READ  getUsername   WRITE setUsername)
  Q_PROPERTY(QString  password  READ  getPassword   WRITE setPassword)


/***** End of: Q_PROPERTY Properties ************************************//*@}*/

public:
  CalendarClient_CalDAV(QObject* parent = NULL);
  ~CalendarClient_CalDAV();

  /**
   * @brief Parses the CALENDAR section of an iCalendar file.
   */
  void parseCALENDAR(QString href);

  /**
   * @brief Parses the VEVENT section of an iCalendar file.
   */
//  void parseVEVENT(QString href);

  /**
   * @brief Adds a single occurrence event to the m_EventList if the event occurs within a given start and end QDateTime.
   * @note Single occurrence event = event without RRULE value.
   * @note Events with a RRULE value are handled within parseVEVENT()
   */
  QList<QObject*> handleSingleEvent(CalendarEvent& evt, const QDateTime& startOfQuestionedDate, const QDateTime& endOfQuestionedDate);

  /**
   * @brief Returns true, if a questioned date is excluded by the event EXDATE value.
   */
  bool isDateExcluded(const QString strExdates, const QDate& questionedDate) const;


  /**
   * @brief Returns a list of events which occur on the questioned date.
   */
//  Q_INVOKABLE QList<QObject*> eventsForDate(const QDate& date);

  /**
   * @brief Returns the complete list of events this CalendarClient manages.
   */
  void allEvents(QList<CalendarEvent*>* events);

/******************************************************************************/
/* Protected function prototypes                                                */
/******************************************************************************/
protected:

  /**
   * @brief Retrieves a sync token from the calDAv server.
   *
   * If the token has changed, the changes shall be retrieved by calling
   * sendRequestChanges().
   */
  void sendRequestSyncToken(void);

  /**
   * @brief Obtains calendar information from the calDAV server.
   */
  void sendRequestChanges(void);

  void setupStateMachine(void);

/******************************************************************************/
/* Private function prototypes                                                */
/******************************************************************************/
private:

/******************************************************************************/
/* Signals                                                                    */
/******************************************************************************/
signals:
  void syncStateChanged(E_CalendarState syncState); // emitted when the calendar has entered a new synchronization state
  void hostURLChanged(QString hostURL);
  void displayNameChanged(QString hostURL);

  // emitted, when the events in m_EventList have changed
  void eventsUpdated(void);

  // emitted, when an error situation occurred
  void error(QString errorMsg);

  // emitted when the error state shall be recovered
  void recoverSignal(void);

  void requestTimeoutMSChanged(int);

  void forceSynchronization(void); // emitted to force a synchronization with the calendar source (file or server)

  void yearChanged(const int& year);
  void monthChanged(const int& month);

  void syncTokenChanged(void);        // emitted when the sync token has changed
  void syncTokenHasNotChanged(void);

  void calendarHasNotChanged(void);
  void calendarUpdateRequired(void); // emitted when the sync token has changed or the year/month since the last synchronization

/******************************************************************************/
/* Public slots                                                               */
/******************************************************************************/
public slots:

  /**
   * @brief Slot to start synchronization with event source.
   */
   void startSynchronization(void);

  /**
   * @brief Slot to stop/abort synchronization with event source.
   */
   void stopSynchronization(void);

  /**
   * @brief Slot to recover from error state.
   */
   void recover(void);

  inline E_CalendarState getSyncState(void){return m_State;};

  bool setHostURL(const QUrl hostURL);
  inline QString getHostURL(void) const{return m_HostURL.toString();};

  int getRequestTimeoutMS(void) const;
  void setRequestTimeoutMS(const int requestTimeoutMS);

  inline int getYear() const {return m_Year;};
  void setYear(const int& year);

  inline int getMonth() const{return m_Month;};
  void setMonth(const int& month);

  void setUsername(const QString username);
  inline QString getUsername(void) const {return m_Username;}

  void setPassword(const QString password);
  inline QString getPassword(void) const {return QString();};

  /**
   * @brief Saves a event to the calDAV server.
   *
   * If the uid parameter is empty, a new event will be created.
   */
  void saveEvent(CalendarEvent* evt);

  /**
   * @brief Deletes a specific event from the calDAV server.
   */
  void deleteEvent(QString href);

/******************************************************************************/
/* Protected slots                                                            */
/******************************************************************************/
protected slots:
  void handleHTTPError(void);

  void handleRequestSyncTokenFinished(void);
  void handleRequestChangesFinished(void);

  void handleStateWaitingEntry(void);
  void handleStateWaitingExit(void);

  void handleStateRequestingSyncTokenEntry(void);
  void handleStateRequestingSyncTokenExit(void);

  void handleStateRequestingChangesEntry(void);
  void handleStateRequestingChangesExit(void);

  void handleStateProcessingChangesEntry(void);
  void handleStateProcessingChangesExit(void);


  void handleStateErrorEntry(void);

  void debug_handleTimerTimeout(void);

  void handleUploadHTTPError(void);
  void handleUploadFinished(void);


/******************************************************************************/
/* Private slots                                                               */
/******************************************************************************/
private slots:

/******************************************************************************/
/* Protected attributes                                                         */
/******************************************************************************/
protected:
  int lastSyncYear;
  int lastSyncMonth;

  int m_YearToBeRequested;
  int m_MonthToBeRequested;
  int m_Year;
  int m_Month;
  QString m_Username;
  QString m_Password;

  QString m_cTag;
  QString m_syncToken;

  bool m_bRecoveredFromError;

  QNetworkAccessManager m_UploadNetworkManager;
  QNetworkReply* m_pUploadReply;
  QTimer m_UploadRequestTimeoutTimer;



  QString               m_Color;
  E_CalendarState       m_State;
//  E_CalendarType        m_CalendarType;
  QUrl  				m_HostURL;
  QString               m_DisplayName;

  // Timer to automatically re-synchronize with event source
  QTimer                m_SynchronizationTimer;

  // Test stream to handle iCalendar file content
  QTextStream*          m_DataStream;

  // List of managed CalendarEvent events
  QList<CalendarEvent*>  m_EventList;

  QNetworkAccessManager m_NetworkManager;
  QNetworkReply* m_pReply;

  QTimer m_RequestTimeoutTimer;
  int m_RequestTimeoutMS;



/******************************************************************************/
/* Private attributes                                                         */
/******************************************************************************/
private:

};

/***** End of: Classes **************************************************//*@}*/

#endif // CALENDARCLIENT_CALDAV_H
/**** Last line of source code                                             ****/






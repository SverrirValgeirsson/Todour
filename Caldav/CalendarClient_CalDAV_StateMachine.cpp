/*********************************************//*!@addtogroup file Files*//*@{*/
/*!
 *******************************************************************************
* File identification:      $Id: CalendarClient_CalDAV_StateMachine.cpp 11 2017-01-16 20:27:13Z cypax $
* Revision of last commit:  $Rev: 11 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-01-16 21:27:13 +0100 (Mo, 16 Jan 2017) $
 *******************************************************************************
 *
 * @file          CalendarClient_CalDAV_StateMachine.cpp
 * @author        Cypax (cypax.net)
 *
 * @brief         State machine implementation of class CalendarClient_CalDAV.
 */
/************************************************************************//*@}*/

/******************************************************************************/
/* Library includes                                                           */
/******************************************************************************/
#include <QDebug>
#include <QStateMachine>  //GDE May2025

/******************************************************************************/
/* Own includes                                                               */
/******************************************************************************/
#include "CalendarClient_CalDAV.h"

/******************************************************************************/
/* Namespace                                                                 */
/******************************************************************************/

/******************************************************************************/
/* Constants (#define)                                                        */
/***************************************//*!@addtogroup define Constants*//*@{*/

#define DEBUG_CALENDARCLIENT_CALDAV_STATEMACHINE 1
#if DEBUG_CALENDARCLIENT_CALDAV_STATEMACHINE
#define QDEBUG qDebug()
#else
#define QDEBUG if (0) qDebug()
#endif

/******************************************************************************/
/* Protected functions                                                        */
/*************************//*!@addtogroup protfunct Protected functions *//*@{*/
void CalendarClient_CalDAV::setupStateMachine(void)
{
  QStateMachine* pStateMachine = new QStateMachine(this);

  QState* pStateWaiting = new QState(pStateMachine);
  QState* pStateRequestingSyncToken = new QState(pStateMachine);
  QState* pStateRequestingChanges = new QState(pStateMachine);
  QState* pStateProcessingChanges = new QState(pStateMachine);
  QState* pStateError = new QState(pStateMachine);

 // connect(&m_SynchronizationTimer, SIGNAL(timeout()), this, SLOT(debug_handleTimerTimeout()));

  // pStateWaiting
  pStateWaiting->addTransition(&m_SynchronizationTimer, SIGNAL(timeout()), pStateRequestingSyncToken);
  pStateWaiting->addTransition(this, SIGNAL(forceSynchronization()), pStateRequestingSyncToken);
  connect(pStateWaiting, SIGNAL(entered()),
          this, SLOT(handleStateWaitingEntry()));
  connect(pStateWaiting, SIGNAL(exited()),
          this, SLOT(handleStateWaitingExit()));


  // pStateRequestingSyncToken
  pStateRequestingSyncToken->addTransition(this, SIGNAL(calendarUpdateRequired()), pStateRequestingChanges);
  pStateRequestingSyncToken->addTransition(this, SIGNAL(calendarHasNotChanged()), pStateWaiting);
  pStateRequestingSyncToken->addTransition(this, SIGNAL(error(QString)), pStateError);
  connect(pStateRequestingSyncToken, SIGNAL(entered()),
          this, SLOT(handleStateRequestingSyncTokenEntry()));
  connect(pStateRequestingSyncToken, SIGNAL(exited()),
          this, SLOT(handleStateRequestingSyncTokenExit()));


  // pStateRequestingChanges
  pStateRequestingChanges->addTransition(this, SIGNAL(calendarUpdateRequired()), pStateProcessingChanges);
  pStateRequestingChanges->addTransition(this, SIGNAL(calendarHasNotChanged()), pStateWaiting);
  pStateRequestingChanges->addTransition(this, SIGNAL(eventsUpdated()), pStateWaiting);
  pStateRequestingChanges->addTransition(this, SIGNAL(error(QString)), pStateError);
  connect(pStateRequestingChanges, SIGNAL(entered()),
          this, SLOT(handleStateRequestingChangesEntry()));
  connect(pStateRequestingChanges, SIGNAL(exited()),
          this, SLOT(handleStateRequestingChangesExit()));


  // pStateProcessingChanges
  pStateProcessingChanges->addTransition(this, SIGNAL(error(QString)), pStateError);
  pStateProcessingChanges->addTransition(this, SIGNAL(eventsUpdated()), pStateWaiting);

  connect(pStateProcessingChanges, SIGNAL(entered()),
          this, SLOT(handleStateProcessingChangesEntry()));
  connect(pStateProcessingChanges, SIGNAL(exited()),
          this, SLOT(handleStateProcessingChangesExit()));


  // pStateError
  pStateError->addTransition(this, SIGNAL(recoverSignal()), pStateWaiting);
  connect(pStateError, SIGNAL(entered()),
          this, SLOT(handleStateErrorEntry()));


  pStateMachine->setInitialState(pStateWaiting);

  pStateMachine->start();
  qDebug()<<"StateMachine started"<<Qt::endl;
}

/***** End of: protfunct Protected functions ****************************//*@}*/

/******************************************************************************/
/* Protected slots                                                            */
/*********************//*!@addtogroup protslots Protected slots         *//*@{*/


void CalendarClient_CalDAV::handleStateWaitingEntry(void)
{
  QDEBUG << m_DisplayName << ": " << "entering pStateWaiting";
  m_State = E_STATE_IDLE;
  emit syncStateChanged(m_State);

  if ( (m_YearToBeRequested != m_Year) || (m_MonthToBeRequested != m_Month) )
  {
    QDEBUG << m_DisplayName << ": " << "year/month has requested from" << m_Year << m_Month << "to" << m_YearToBeRequested << m_MonthToBeRequested << "=> update required";
    setYear(m_YearToBeRequested);
    setMonth(m_MonthToBeRequested);
    startSynchronization();
  }
  else if (false != m_bRecoveredFromError)
  {
    QDEBUG << m_DisplayName << ": " << "recovery from ERROR state => update required";
    m_bRecoveredFromError = false;
    startSynchronization();
  }
}

void CalendarClient_CalDAV::handleStateWaitingExit(void)
{
  m_State = E_STATE_BUSY;
  emit syncStateChanged(m_State);
  QDEBUG << m_DisplayName << ": " << "leaving pStateWaiting";
}

void CalendarClient_CalDAV::handleStateRequestingSyncTokenEntry(void)
{
  QDEBUG << m_DisplayName << ": " << "entering pStateRequestingSyncToken";
  sendRequestSyncToken();
}

void CalendarClient_CalDAV::handleStateRequestingSyncTokenExit(void)
{
  QDEBUG << m_DisplayName << ": " << "leaving pStateRequestingSyncToken";
}

void CalendarClient_CalDAV::handleStateRequestingChangesEntry(void)
{
  QDEBUG << m_DisplayName << ": " << "entering pStateRequestingChanges";
  sendRequestChanges();
}

void CalendarClient_CalDAV::handleStateRequestingChangesExit(void)
{
  QDEBUG << m_DisplayName << ": " << "leaving pStateRequestingChanges";
}


void CalendarClient_CalDAV::handleStateProcessingChangesEntry(void)
{
  QDEBUG << m_DisplayName << ": " << "entering pStateProcessingChanges";
}

void CalendarClient_CalDAV::handleStateProcessingChangesExit(void)
{
  QDEBUG << m_DisplayName << ": " << "leaving pStateProcessingChanges";
}




void CalendarClient_CalDAV::handleStateErrorEntry(void)
{
  QDEBUG << m_DisplayName << ": " << "entering state error";
}


void CalendarClient_CalDAV::debug_handleTimerTimeout(void)
{
  QDEBUG << m_DisplayName << ": " << "~~~~~~sync timer timeout~~~~~~";
}

/***** End of: protslots Protected slots ********************************//*@}*/

/******************************************************************************/
/* Private slots                                                              */
/*************************//*!@addtogroup privslots Private slots       *//*@{*/
/***** End of:privslots Private slots ***********************************//*@}*/

/**** Last line of source code                                             ****/

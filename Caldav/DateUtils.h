/*********************************************//*!@addtogroup file Files*//*@{*/
/*!
 *******************************************************************************
* File identification:      $Id: DateUtils.h 11 2017-01-16 20:27:13Z cypax $
* Revision of last commit:  $Rev: 11 $
* Author of last commit:    $Author: cypax $
* Date of last commit:      $Date: 2017-01-16 21:27:13 +0100 (Mo, 16 Jan 2017) $
 *******************************************************************************
 *
 * @author        Cypax (cypax.net)
 *
 * @brief         Common date and time utils.
 */
/************************************************************************//*@}*/

#ifndef DATEUTILS_H
#define DATEUTILS_H

/******************************************************************************/
/* Library includes                                                           */
/******************************************************************************/
#include <QDateTime>
#include <QDate>

/******************************************************************************/
/* Own includes                                                               */
/******************************************************************************/

/******************************************************************************/
/* Namespace                                                                 */
/******************************************************************************/

/******************************************************************************/
/* Constants (#define)                                                        */
/***************************************//*!@addtogroup define Constants*//*@{*/

#define DATEUTILS_INVALID_DAY 32

/***** End of: define Constants *****************************************//*@}*/

/******************************************************************************/
/* Class                                                                      */
/******************************************//*!@addtogroup class Classes*//*@{*/
/**
* @brief DateUtils provides (static) methods for date and time handling.
**/
class DateUtils
{

/******************************************************************************/
/* Types (typedef)                                                            */
/******************************************//*!@addtogroup typedef Types*//*@{*/
public:

/***** End of: typedef Types ********************************************//*@}*/

/******************************************************************************/
/* Properties (Q_PROPERTY)                                                    */
/**********************************//*!@addtogroup Q_PROPERTY Properties*//*@{*/
/***** End of: Q_PROPERTY Properties ************************************//*@}*/

/******************************************************************************/
/* Constructors                                                               */
/******************************************************************************/
public:

/******************************************************************************/
/* Destructor                                                                 */
/******************************************************************************/
public:

/******************************************************************************/
/* Public function prototypes                                                 */
/******************************************************************************/
public:

  /**
   * @brief Returns the weekday index (1..7) from a string.
   * @param weekdayString Should be like "2FR" (2nd Friday), "-1SA" (last Saturday), "MO" (Monday).
   * @note Parameter weekdayString is not case sensitive.
   * @retval Returns 0 if weekdayString is invalid.
   */
  static int getWeekdayIndexFromString(QString weekdayString);

  /**
   * @brief Returns the integer part from strings like "2FR" (2nd Friday) or "-1SA" (last Saturday).
   * @note Parameter weekdayString is not case sensitive.
   * @retval Returns DATEUTILS_INVALID_DAY if weekdayString is invalid.
   * @retval Returns 0 if weekdayString contains no integer part.
   */
  static int getWeekdaySelectorFromString(QString weekdayString);

  /**
   * @brief Returns the number of occurrences of a weekday (1..7 = MO..SU) in a given month.
   * @retval Returns -1 if iWeekday is invalid.
   */
  static int getWeekdayCountInMonth(int iWeekday, QDate monthDate);

  /**
   * @brief Returns a QDate of a given weekday (1-7) in a given month and with respect to a given weekday selector.
   * @param iWeekDaySelector Example: -2 = second-last occurrence of iWeekday in month
   * @retval Returns an invalid QDate if the questioned weekday occurrence is not contained in the month (i.e. if asked for the 10th Monday)
   */
  static QDate getWeekdayOfMonth(int iWeekday, int iWeekDaySelector, QDate monthDate);

  /**
   * @brief Helper function to get the last day of a year+month.
   */
  static int lastDayOfMonth(int year, int month);
  
  static QDateTime parseDate(const QString strExdates);

  static bool isDateExcluded(const QString strExdates, const QDate& questionedDate);

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

/******************************************************************************/
/* Public slots                                                               */
/******************************************************************************/
public slots:

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

/******************************************************************************/
/* Private attributes                                                         */
/******************************************************************************/
private:

};

/***** End of: Classes **************************************************//*@}*/

#endif // DATEUTILS_H
/**** Last line of source code                                             ****/






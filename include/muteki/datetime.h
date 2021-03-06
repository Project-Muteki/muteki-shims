/**
 * @file datetime.h
 * @brief Datetime operations.
 */

#ifndef __MUTEKI_DATETIME_H__
#define __MUTEKI_DATETIME_H__

/**
 * @brief The datetime struct used by GetSysTime and SetSysTime.
 */
typedef struct {
    /**
     * @brief Year.
     */
    short year;
    /**
     * @brief Month of year.
     */
    short month;
    /**
     * @brief Day of week (0..6). 0 is Sunday.
     */
    short weekday;
    /**
     * @brief Day of month.
     */
    short day;
    /**
     * @brief Number of hours past midnight.
     */
    short hour;
    /**
     * @brief Number of minutes after the hour.
     */
    short minute;
    /**
     * @brief Number of seconds after the minute.
     */
    short second;
    /**
     * @brief Number of milliseconds after the second.
     */
    short millis;
} datetime_t;

/**
 * @brief Read current time of the day and store it in @p dt.
 *
 * @param dt Pointer to an empty datetime struct.
 */
extern void GetSysTime(datetime_t *dt);

#endif // __MUTEKI_DATETIME_H__

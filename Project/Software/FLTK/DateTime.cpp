//
// forked from https://github.com/hasenradball/DS3231-RTC
// by @hasenradball
// MIT License
//

#include <cstdio>
#include <cstring>
#include "DateTime.h"


// *****************************************
//  Static Functions only used in this file
// *****************************************

static const uint8_t daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
#define UNIX_OFFSET 946684800UL

/**
 * @brief function which calculates if a year is a leap year
 *
 * @param year
 * @return true
 * @return false
 */
static bool isleapYear(const int16_t year) {
    // check if divisible by 4
    if(year % 4) {
        return false;
    }
    // only check other, when first failed
    return (year % 100 || year % 400 == 0);
}

/**
 * @brief calculate the days since January 1 (0...365)
 *
 * @param year e.g.: 2022
 * @param month 1...12
 * @param day 1...31
 * @return int16_t
 */
/*static int16_t calcYearDay(const int16_t year, const int8_t month, const int8_t day) {
    uint16_t days = day - 1;
    for (uint8_t i = 1; i < month; ++i)
        days += pgm_read_byte(daysInMonth + i - 1);
    if (month > 2 && isleapYear(year))
        ++days;
    return days;
}*/

// Slightly modified from JeeLabs / Ladyada
// Get all date/time at once to avoid rollover (e.g., minute/second don't match)
// Commented to avoid compiler warnings, but keeping in case we want this
// eventually
// static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }
static uint8_t bcd2bin (uint8_t val) {
    return val - 6 * (val >> 4);
}


// *****************************************
//   Member functions for DateTime object
// *****************************************
/**
 * @brief Construct a new Date Time:: Date Time object
 *
 * @param timestamp
 */
DateTime::DateTime (time_t unix_timestamp)
        : _unix_timestamp{unix_timestamp}, _y2k_timestamp{unix_timestamp - UNIX_OFFSET}
{
    gmtime_r(&_unix_timestamp, &_tm);
}

/**
 * @brief Construct a new Date Time:: Date Time object
 *
 * @param year year e.g. 2022
 * @param month months since January - [ 1...12 ]
 * @param day day of the month - [ 1...31 ]
 * @param hour hours since midnight - [ 0...23 ]
 * @param min minutes after the hour - [ 0...59 ]
 * @param sec seconds after the minute - [ 0...59 ]
 * @param wday wdays since Sunday - [ 1...7 ]
 * @param dst Daylight Saving Time flag
 */
DateTime::DateTime(int16_t year, int8_t month, int8_t day, int8_t hour, int8_t min, int8_t sec, int8_t wday, int16_t dst)
{
    _tm.tm_sec = sec;
    _tm.tm_min = min;
    _tm.tm_hour = hour;
    _tm.tm_mday = day;
    _tm.tm_mon = month;
    _tm.tm_year = year;
    _tm.tm_wday = wday;
    _tm.tm_isdst = dst;

    set_timstamps();
}

/**
 * @brief Construct a new Date Time:: Date Time object by given the precompiler marcos
 * as __DATE__ and __TIME__
 *
 * @param date as  Mmm dd yyyy (e.g. "Jan 14 2012")
 * @param time as HH:MM:SS (e.g. "23:59:01")
 */
 DateTime::DateTime(const char *date, const char *time) {
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    static const char my_array[5] = {'a', 'b', 'c', 'd', 'e'};
    static char month_buff[4] = {'0','0','0','0'};
    int year, day, hour, min, sec;
    // the the formated data string and store the value in month_buff, day and year
    sscanf(date, "%s %2d %4d", month_buff, &day, &year);
    int month = (strstr(month_names, month_buff) - month_names) / 3 + 1;
    _tm.tm_year = year;
    _tm.tm_mon = month;
    _tm.tm_mday = day;
    //byte hour, min, sec;
    sscanf(time, "%hhu:%hhu:%hhu", &hour, &min, &sec);
    _tm.tm_hour = hour;
    _tm.tm_min = min;
    _tm.tm_sec = sec;
    set_timstamps();
}

/**
 * @brief Set the timestamps by using struct tm entries
 *
 */
void DateTime::set_timstamps() {
#if defined (__AVR__)
    _y2k_timestamp = mktime(&_tm);
    _unix_timestamp = _y2k_timestamp + UNIX_OFFSET;
#else
    _unix_timestamp = mktime(&_tm);
    _y2k_timestamp = _unix_timestamp - UNIX_OFFSET;
#endif
}

/**
 * @brief function to format a DateTime string in an buffer based on the standard strftime function
 *
 *  see: https://cplusplus.com/reference/ctime/strftime/
 *  or:  https://en.cppreference.com/w/cpp/chrono/c/strftime
 *
 * @param buffer buffer for time string
 * @param buffersize size of buffer
 * @param formatSpec define format see strftime
 * @return size_t length of used buffer
 */
size_t DateTime::strf_DateTime(char *buffer, size_t buffersize, const char *formatSpec) {
    size_t len {strftime(buffer, buffersize, formatSpec, &_tm)};
    return len;
}

std::vector<uint8_t> DateTime::getDatetimeAsVector(void) {
    std::vector<uint8_t> datetime;
    datetime.push_back(_tm.tm_sec);
    datetime.push_back(_tm.tm_min);
    datetime.push_back(_tm.tm_hour);
    datetime.push_back(_tm.tm_wday);
    datetime.push_back(_tm.tm_mday);
    datetime.push_back(_tm.tm_mon);
    datetime.push_back(_tm.tm_year);

    return datetime;
}



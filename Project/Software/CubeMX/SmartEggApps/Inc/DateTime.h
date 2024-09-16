#ifndef DISPLAYTEST3_DATETIME_H
#define DISPLAYTEST3_DATETIME_H


#include <ctime>
#include <vector>

// DateTime class restructured by using standardized time functions
class DateTime {
public:
    DateTime (time_t unix_timestamp = 0);

    DateTime ( 	int16_t year, int8_t month, int8_t mday,
                  int8_t hour = 0, int8_t min = 0, int8_t sec = 0,
                  int8_t wday = 0, int16_t dst = -1);

    DateTime (const char *date, const char *time);

    int16_t getYear()   const	{ return _tm.tm_year ; }
    int8_t getMonth()   const   { return _tm.tm_mon + 1; }
    int8_t getDay()     const   { return _tm.tm_mday; }
    int8_t getHour()    const   { return _tm.tm_hour; }
    int8_t getMinute()  const   { return _tm.tm_min; }
    int8_t getSecond()  const   { return _tm.tm_sec; }
    int8_t getWeekDay() const	{ return _tm.tm_wday; }
    int16_t getYearDay() const	{ return _tm.tm_yday; }
    int16_t getDST()    const   { return _tm.tm_isdst; }
    size_t strf_DateTime(char *buffer, size_t buffersize, const char *formatSpec = "%a %h %d %T %Y");

    // time_t value as seconds since 1/1/2000
    time_t getY2kTime() const { return _y2k_timestamp; }

    // time_t value as seconds since 1/1/1970
    // THE ABOVE COMMENT IS CORRECT FOR LOCAL TIME; TO USE THIS COMMAND TO
    // OBTAIN TRUE UNIX TIME SINCE EPOCH, YOU MUST CALL THIS COMMAND AFTER
    // SETTING YOUR CLOCK TO UTC
    time_t getUnixTime() const  { return _unix_timestamp; }

    std::vector<uint8_t> getDatetimeAsVector(void);

private:
    void set_timstamps();

protected:
    time_t _unix_timestamp;
    time_t _y2k_timestamp;
    struct tm _tm;
};


#endif //DISPLAYTEST3_DATETIME_H

#ifndef DISPLAYTEST3_DS3231_H
#define DISPLAYTEST3_DS3231_H

#include <ctime>
#include <cstdio>
#include "i2c_sensor.h"
#include "main.h"
#include "DateTime.h"


class DS3231: public i2c_sensor {
public:
    // Constructor
    DS3231();
    ~DS3231() = default;

    enum regAddr : uint8_t
    {
        SECOND          = 0x00,
        MINUTE          = 0x01,
        HOUR            = 0x02,
        DAY_OF_WEEK     = 0x03,
        DAY_OF_MONTH    = 0x04,
        MONTH           = 0x05,
        YEAR            = 0x06,
        TEMPERATURE_MSB = 0x11,
        TEMPERATURE_LSB = 0x12,
        CONTROL         = 0x0E,
        CONTROL_STATUS = 0x0F

    };
public:

    // ************************************
    //      Time-retrieval functions
    // ************************************

    /**
     * get the current time and as Datetime object
     * @return the current time and date
     */
    DateTime getCurrentTime();

    // Get the second of the DS3231 module
    /**
     * get the current second
     * @return current second [1..59]
     */
    int8_t getSecond();

    // Get the minute of the DS3231 module
    /**
     * get the current minute
     * @return current minute [1..59]
     */
    int8_t getMinute();


    /**
     * get the current hour.
     * @return the current hour [0..12] if AM/PM flag is set, else [0..24]
     */
    int8_t getHour();

    /**
     * get the day of the week, starting at monday? @todo
     * @return day of the week [1..7]
     */
    int8_t getDayOfWeek();

    /**
     * get the day of the month
     * @return day of the month [1..31]
     */
    int8_t getDayOfMonth();


    /**
     * get the month and the century roll over
     * @param century true, if their was a roll over
     * @return the current month [1..12]
     */
    int8_t getMonth(bool &century);


    /**
     * get the current year
     * @return the current year [2000..2099]
     */
    int16_t getYear();


    // ************************************
    //        Time-setting functions
    // ************************************
    // Note that none of these check for sensibility: You can set the
    // date to July 42nd and strange things will probably result.

    /**
      * set the given datatime as new datetime
      * @param currentTime the current time to set for the RTC
      */
    void setCurrentTime(DateTime currentTime);

    // set epoch function gives the epoch as parameter and feeds the RTC
    // epoch = UnixTime and starts at 01.01.1970 00:00:00
    // void setEpoch(time_t epoch = 0, bool flag_localtime = false);

    // Set the Second of the DS3231 module
    void setSecond(int8_t second);

    // Set the minute of the DS3231 module
    void setMinute(int8_t minute);

    // Set the hour of the DS3231 module
    void setHour(int8_t hour);

    // Sets the Day of the Week (1...7) of the DS3231 module
    void setDoW(int8_t dayOfWeek);

    // Sets the Date of the DS3231 module
    void setDate(int8_t date);

    // Sets the Month of the DS3231 module
    void setMonth(int8_t month);

    // Sets the Year of the DS3231 module
    void setYear(int8_t year);

    // Sets the Hour format (12h/24h) of the DS3231 module
    void setClockMode(bool h12);

    // can be called on a regular basis to check if we have to switch to summertime
    // if this is the case, the clock is automatically changed
    bool checkIfWeHaveToSwitchToSummertime();

    /**
     * calculate the DST for germany with summer and wintertime support
     * @param month the current month (1..12)
     * @param wday the current day of the week (1..7)
     * @param date the current day of the month (1..31)
     * @param hour the current hour of the day (0..24)
     * @return 2 or 1 corresponding to summer/wintertime
     */
    int16_t calculateDSTWithSummerAndWintertime(int8_t month, int8_t wday, int8_t date, int8_t hour);


    // ************************************
    //        Temperature function
    // ************************************
    // get temperature of the DS3231 module
    float getTemperature();


    // ***********************************
    //          Alarm functions
    // ***********************************
    void getA1Time(std::byte& A1Day, std::byte & A1Hour, std::byte & A1Minute, std::byte & A1Second, std::byte & AlarmBits, bool& A1Dy, bool& A1h12, bool& A1PM);

    /* Retrieves everything you could want to know about alarm
    * one.
    * A1Dy true makes the alarm go on A1Day = Day of Week,
    * A1Dy false makes the alarm go on A1Day = Date of month.
    *
    * std::byte AlarmBits sets the behavior of the alarms:
    *	Dy	A1M4	A1M3	A1M2	A1M1	Rate
    *	X	1		1		1		1		Once per second
    *	X	1		1		1		0		Alarm when seconds match
    *	X	1		1		0		0		Alarm when min, sec match
    *	X	1		0		0		0		Alarm when hour, min, sec match
    *	0	0		0		0		0		Alarm when date, h, m, s match
    *	1	0		0		0		0		Alarm when DoW, h, m, s match
    *
    *	Dy	A2M4	A2M3	A2M2	Rate
    *	X	1		1		1		Once per minute (at seconds = 00)
    *	X	1		1		0		Alarm when minutes match
    *	X	1		0		0		Alarm when hours and minutes match
    *	0	0		0		0		Alarm when date, hour, min match
    *	1	0		0		0		Alarm when DoW, hour, min match
    *
    *	Note: std::std::byte AlarmBits is not explicitly cleared for the getAXTime methods to
    *	support sequential retrieval of both alarms with the same std::byte AlarmBits.
    *	Use the flag bool clearAlarmBits=True to explicitly clear std::byte AlarmBits on
    *  call to getAXTime.
    */

    // Same as getA1Time();, but A2 only goes on seconds == 00.
    void getA2Time(std::byte& A2Day, std::byte& A2Hour, std::byte& A2Minute, std::byte& AlarmBits, bool& A2Dy, bool& A2h12, bool& A2PM);

    // Same as getA1Time();, but clears std::byte AlarmBits.
    void getA1Time(std::byte& A1Day, std::byte& A1Hour, std::byte& A1Minute, std::byte& A1Second, std::byte& AlarmBits, bool& A1Dy, bool& A1h12, bool& A1PM, bool clearAlarmBits);

    // Same as getA1Time();, but clears std::byte AlarmBits.
    void getA2Time(std::byte& A1Day, std::byte& A1Hour, std::byte& A1Minute,std::byte& AlarmBits, bool& A1Dy, bool& A1h12, bool& A1PM, bool clearAlarmBits);

    // Set the details for Alarm 1
    void setA1Time(std::byte A1Day, std::byte A1Hour, std::byte A1Minute, std::byte A1Second, std::byte AlarmBits, bool A1Dy, bool A1h12, bool A1PM);

    // Set the details for Alarm 2
    void setA2Time(std::byte A2Day, std::byte A2Hour, std::byte A2Minute, std::byte AlarmBits, bool A2Dy, bool A2h12, bool A2PM);

    // Enables alarm 1 or 2 and the external interrupt pin.
    // If Alarm != 1, it assumes Alarm == 2.
    void turnOnAlarm(std::byte Alarm);

    // Disables alarm 1 or 2 (default is 2 if Alarm != 1);
    // and leaves the interrupt pin alone.
    void turnOffAlarm(std::byte Alarm);

    // Returns T/F to indicate whether the requested alarm is
    // enabled. Defaults to 2 if Alarm != 1.
    bool checkAlarmEnabled(std::byte Alarm);

    // Checks whether the indicated alarm (1 or 2, 2 default);
    // has been activated. Always clears flag.
    bool checkIfAlarm(std::byte Alarm);

    // Checks whether the indicated alarm (1 or 2, 2 default);
    // has been activated. IF clearflag is set, clears alarm flag.
    bool checkIfAlarm(std::byte Alarm, bool clearflag);


    // ************************************
    //        Oscillator functions
    // ************************************

    // turns oscillator on or off. True is on, false is off.
    // if battery is true, turns on even for battery-only operation,
    // otherwise turns off if Vcc is off.
    // frequency must be 0, 1, 2, or 3.
    // 0 = 1 Hz
    // 1 = 1.024 kHz
    // 2 = 4.096 kHz
    // 3 = 8.192 kHz (Default if frequency std::byte is out of range);
    void enableOscillator(bool TF, bool battery, uint8_t frequency);

    // Turns the 32kHz output pin on (true); or off (false).
    void enable32kHz(bool TF);

    // Checks the status of the OSF (Oscillator Stop Flag);.
    // If this returns false, then the clock is probably not
    // giving you the correct time.
    // The OSF is cleared by function setSecond();.
    bool oscillatorCheck();


private:
    // Convert normal decimal numbers to binary coded decimal
    static int8_t decToBcd(int8_t val);
    // Convert binary coded decimal to normal decimal numbers
    static int8_t bcdToDec(int8_t val);
};


#endif //DISPLAYTEST3_DS3231_H

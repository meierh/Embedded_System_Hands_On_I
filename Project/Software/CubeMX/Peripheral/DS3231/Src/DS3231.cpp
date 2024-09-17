#include "DS3231.h"

#define DS3231_ADDRESS   0x68 //0b0011100
#define CLOCK_ADDRESS 0x68 //?

//bool DS3231::alreadyReset = false;

DS3231::DS3231() : i2c_sensor(DS3231_ADDRESS)
{
    // set clock to 24h
    setClockMode(false);
    // enable clock
    enableOscillator(true, true, 3);
}

DateTime DS3231::getCurrentTime() {
    std::vector<uint8_t> senorData;
    readVector(senorData, SECOND, 6);
    int8_t sec = bcdToDec(senorData[0]);
    int8_t min = bcdToDec(senorData[1]);
    int8_t hour = bcdToDec(senorData[2]);
    int8_t dayOfWeek = bcdToDec(senorData[3]);
    int8_t dayOfMonth = bcdToDec(senorData[4]);
    int8_t  month = bcdToDec(senorData[5]);
    // the rtc stores only the last o digits of the year. So we have to add 2000
    int16_t year = bcdToDec(senorData[6])+2000;
    int16_t dst = calculateDSTWithSummerAndWintertime(month, dayOfWeek, dayOfMonth, hour); // Daylight saving time aka timezone

    return DateTime{year, month, dayOfMonth, hour, min, sec, dayOfWeek, dst};
}


void DS3231::setCurrentTime(DateTime currentTime) {
    static std::vector<uint8_t> dateTimeVector = currentTime.getDatetimeAsVector();
    writeVector(dateTimeVector, SECOND);
}


int8_t DS3231::getSecond() {
    std::vector<uint8_t> senorData;
    readVector(senorData, SECOND, 1);

    return bcdToDec(senorData[0]);
}

int8_t DS3231::getMinute() {
    std::vector<uint8_t> senorData;
    readVector(senorData, MINUTE, 1);
    return bcdToDec(senorData[0]);
}

int8_t DS3231::getHour() {
    std::vector<uint8_t> senorData;
    readVector(senorData, HOUR, 1);

    return bcdToDec(senorData[0]);
}

int8_t DS3231::getDayOfWeek() {
    std::vector<uint8_t> senorData;
    readVector(senorData, DAY_OF_WEEK, 1);
    return bcdToDec(senorData[0]);
}

int8_t DS3231::getDayOfMonth() {
    std::vector<uint8_t> senorData;
    readVector(senorData, DAY_OF_MONTH, 1);
    return bcdToDec(senorData[0]);
}

int8_t DS3231::getMonth(bool &century) {
    std::vector<uint8_t> senorData;
    readVector(senorData, MONTH, 1);
    century = senorData[0] & 0b10000000;
    return bcdToDec(senorData[0]);
}

int16_t DS3231::getYear() {
    std::vector<uint8_t> senorData;
    readVector(senorData, YEAR, 1);
    return bcdToDec(senorData[0]) + 2000;
}

void DS3231::setSecond(int8_t second) {
    writeByte(second, SECOND);
    //@todo clear OSF flag?
}

void DS3231::setMinute(int8_t minute) {
    writeByte(minute, MINUTE);
}

void DS3231::setHour(int8_t hour) {
    writeByte(hour, HOUR);

}

void DS3231::setDoW(int8_t dayOfWeek) {
    writeByte(dayOfWeek, DAY_OF_WEEK);
}

void DS3231::setDate(int8_t date) {
    writeByte(date, DAY_OF_MONTH);
}

void DS3231::setMonth(int8_t month) {
    writeByte(month, MONTH);
}

void DS3231::setYear(int8_t year) {
    writeByte(year, YEAR);
}

float DS3231::getTemperature() {
    std::vector<uint8_t> sensorData;
    readVector(sensorData, TEMPERATURE_MSB, 2);
    float temperature;
    temperature = (sensorData[0] << 8 | sensorData[1]);
    //@todo temp is two compl number , check it
    return temperature;
}

/**
 * check OSF flag. Returns true if the Oscillator is running, false if not
 * @return True if oscillator is running, false if not
 */
bool DS3231::oscillatorCheck() {
    uint8_t statusByte = readByte(CONTROL_STATUS);
    // the OSF is in the BIT 7 of the Control_Status register
    uint8_t osf = 0b10000000;
    if(statusByte & osf) {
        // OSF flag is set to 1 - the oscillator is stopped
        return true;
    } else {
        // flag ist set to 0 - the oscillator is running
        return false;
    }
}

/**
 * enable/disable the oscillator
 * @param TF True/False if the oscillator should be enabled or disabled
 * @param battery True
 * @param frequency frequency must be 0, 1, 2, or 3.
    \n 0 = 1 Hz
    \n 1 = 1.024 kHz
    \n 2 = 4.096 kHz
    \n 3 = 8.192 kHz (Default if frequency byte is out of range)
 */
void DS3231::enableOscillator(bool TF, bool battery, uint8_t frequency) {
    // turns oscillator on or off. True is on, false is off.
    // if battery is true, turns on even for battery-only operation,
    // otherwise turns off if Vcc is off.
    //
    if (frequency > 3) frequency = 3;
    // read control byte in, but zero out current state of RS2 and RS1.
    uint8_t  temp_buffer = readByte(CONTROL) & 0b11100111;
    if (battery) {
        // turn on BBSQW (Battery-Backed Square-Wave Enable) flag
        temp_buffer = temp_buffer | 0b01000000;
    } else {
        // turn off BBSQW flag
        temp_buffer = temp_buffer & 0b10111111;
    }
    if (TF) {
        // set ~EOSC (Enable Oscillator) to 0 and INTCN (Interrupt Control) to zero.
        temp_buffer = temp_buffer & 0b01111011;
    } else {
        // set ~EOSC to 1, leave INTCN as is.
        temp_buffer = temp_buffer | 0b10000000;
    }
    // shift frequency into bits 3 and 4 and set.
    frequency = frequency << 3;
    temp_buffer = temp_buffer | frequency;
    // And write the control bits
    writeByte(temp_buffer, CONTROL);
}

void DS3231::setClockMode(bool h12) {
    uint8_t temp = readByte(HOUR);

    // set the flag to 12h or 24h
    if(h12) {
        temp = temp | 0b01000000;
    } else {
        temp = temp & 0b10111111;
    }
    writeByte(temp, HOUR);
}

int8_t DS3231::decToBcd(int8_t val) {
    // Convert normal decimal numbers to binary coded decimal
    return ( (val/10*16) + (val%10) );
}

int8_t DS3231::bcdToDec(int8_t val) {
    // Convert binary coded decimal to normal decimal numbers
    return ( (val/16*10) + (val%16) );
}


int16_t DS3231::calculateDSTWithSummerAndWintertime(int8_t month, int8_t wday, int8_t date, int8_t hour) {
    // summertime starts at the last sunday in March at 2:00 -> the clock is set from 2:00 to 3:00
    // wintertime starts at the last sunday in October at 3:00 -> the clock is set from 3:00 to 2:00
    // set to summertime between april and september
    if(month > 3 && month < 10) {
        // summertime UTC+2
        return 2;
    }
    // set to wintertime between November and February
    else if(month > 10 || month < 3) {
        // wintertime UTC+1
        return 1;
    }
    // if it is march, check if the last sunday passed to set to summertime
    else if(month == 3) {
        if(wday == 7 && date > 24 && hour >= 2) {
            // summertime UTC+2
            return 2;
        } else {
            // wintertime UTC+1
            return 1;
        }
    }
    // if it is October, check if the last sunday passed to set to wintertime
    else if(month == 10) {
        if(wday == 7 && date > 24 && hour >= 2) {
            // wintertime UTC+1
            return 1;
        } else {
            // summertime UTC+2
            return 2;
        }
    }
    return 1;
}







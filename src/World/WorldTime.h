#ifndef _WORLD_TIME_H
#define _WORLD_TIME_H

#include "../Utils/utils.h"

#define TIME_COMPRESSION		1000.0f	// FAST TIME
//#define TIME_COMPRESSION		50.0f
#define SECONDS_PER_MINUTE				60
#define MINUTES_PER_HOUR				60
#define HOURS_PER_DAY					24
#define DAYS_PER_MONTH					30
#define MONTHS_PER_YEAR					12

// hardcode constants to avoid calculations and casts
// remember to update it in case above constants would be modified
#define F_MINUTES_PER_HOUR				60.0f
#define F_SECONDS_PER_HOUR				3600.0f

#define TIME_IN_HOURS(dt)				((double)dt.hour + (double)dt.minute / F_MINUTES_PER_HOUR + (double)dt.second / F_SECONDS_PER_HOUR)

class DateTime
{
public:
	DateTime() {
		year = month = day = hour = minute = second = 0;
	}

	DateTime(int year, int month, int day, int hour, int minute, int second) {
		this->year = year;
		this->month = month;
		this->day = day;
		this->hour = hour;
		this->minute = minute;
		this->second = second;
	}

    friend ostream& operator<< (ostream& stream, const DateTime& dt) {
    	stream << dt.year << "-" << dt.month+1 << "-" << dt.day+1 << " " << dt.hour << ":" << dt.minute << ":" << dt.second;
    	return stream;
    };

    void add(int seconds) {
		second += seconds;
		while (second >= SECONDS_PER_MINUTE) {
			second -= SECONDS_PER_MINUTE;
			minute++;
			while (minute >= MINUTES_PER_HOUR) {
				minute -= MINUTES_PER_HOUR;
				hour++;
    			while (hour >= HOURS_PER_DAY) {
    				hour -= HOURS_PER_DAY;
    				day++;
        			while (day >= DAYS_PER_MONTH) {
        				day -= DAYS_PER_MONTH;
        				month++;
            			while (month >= MONTHS_PER_YEAR) {
            				month -= MONTHS_PER_YEAR;
            				year++;
            			}
        			}
    			}
			}
		}
    }

    DateTime minus(DateTime& other) {
    	return DateTime::fromSecondsBased(getSecondsBased() - other.getSecondsBased());
    }

    double getHoursBased(bool maskSeconds, bool maskMinutes, bool maskHours, bool maskDays, bool maskMonths, bool maskYears) {
    	double seconds = maskSeconds ? 0 : ((double)second / (double)SECONDS_PER_MINUTE) / (double)MINUTES_PER_HOUR;
    	double minutes = maskMinutes ? 0 : (double)minute / (double)MINUTES_PER_HOUR;
    	double hours = maskHours ? 0 : (double)hour;
    	double days = maskDays ? 0 : (double) (day * HOURS_PER_DAY);
    	double months = maskMonths ? 0 : (double) (month * DAYS_PER_MONTH * HOURS_PER_DAY);
    	double years = maskYears ? 0 : (double) (year * MONTHS_PER_YEAR * DAYS_PER_MONTH * HOURS_PER_DAY);
    	return seconds + minutes + hours + days + months + years;
    }

    long getSecondsBased(bool maskSeconds = false, bool maskMinutes = false, bool maskHours = false, bool maskDays = false, bool maskMonths = false, bool maskYears = false) const {
    	long seconds = maskSeconds ? 0 : second;
    	long mult = SECONDS_PER_MINUTE;
    	seconds += maskMinutes ? 0 : minute * mult;
    	mult *= MINUTES_PER_HOUR;
    	seconds += maskHours ? 0 : hour * mult;
    	mult *= HOURS_PER_DAY;
    	seconds += maskDays ? 0 : day * mult;
    	mult *= DAYS_PER_MONTH;
    	seconds += maskMonths ? 0 : month * mult;
    	mult *= MONTHS_PER_YEAR;
    	seconds += maskYears ? 0 : year * mult;
    	return seconds;
    }

    static DateTime fromSecondsBased(long seconds) {
    	DateTime dateTime;
    	long diviser = MONTHS_PER_YEAR * DAYS_PER_MONTH * HOURS_PER_DAY * MINUTES_PER_HOUR * SECONDS_PER_MINUTE;
    	dateTime.year = seconds / diviser;
    	seconds = seconds % diviser;
    	diviser /= MONTHS_PER_YEAR;
    	dateTime.month = seconds / diviser;
    	seconds = seconds % diviser;
    	diviser /= DAYS_PER_MONTH;
    	dateTime.day = seconds / diviser;
    	seconds = seconds % diviser;
    	diviser /= HOURS_PER_DAY;
    	dateTime.hour = seconds / diviser;
    	seconds = seconds % diviser;
    	diviser /= MINUTES_PER_HOUR;
    	dateTime.minute = seconds / diviser;
    	seconds = seconds % diviser;
    	diviser /= SECONDS_PER_MINUTE;
    	dateTime.second = seconds;
    	return dateTime;
    }

	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
};

class WorldTime
{
public:
    static WorldTime * getInstance()
    {
        if (m_pInstance == NULL) m_pInstance = new WorldTime();
        return m_pInstance;
    };

    const DateTime& getTime() {
    	return dateTime;
    }

    DateTime getTimeCopy() {
    	return dateTime;
    }

    void add(double delta) {
    	timeBuffer += delta * TIME_COMPRESSION;
    	if (timeBuffer >= 1) {
    		int seconds = (int) timeBuffer;
    		timeBuffer -= (double) seconds;
    		dateTime.add(seconds);
    	}
    };

private:
	WorldTime() {
		timeBuffer = 0;
		dateTime.hour = 6;
	};
    static WorldTime * m_pInstance;

    DateTime dateTime;
    double timeBuffer;
};

// Helper alias
#define _time WorldTime::getInstance()

#endif

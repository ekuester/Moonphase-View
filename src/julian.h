/* 
 * File:   julian.h
 * Author: kuestere
 *
 * Created on 8. August 2018, 13:40
 */

#ifndef JULIAN_H
#define JULIAN_H

#include <ctime>
#include <gtkmm.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <tuple>

using namespace std;

// julian day (days since midday of January 1, -4712)
// implements a Julian calendar with year 0
// see Jean Meeus, Astronomische Algorithmen,1992, p. 72 ff.
// see <http://howardhinnant.github.io/date_algorithms.html>
// first day 01/01/0001 which means 306.0 of proleptic julian calendar is MONDAY
//<<<< to be examined !!!
class Julian {
public:
    // Default constructor: today's date and time as julian day number
    Julian();

    // simply construct from valid julian number
    Julian(double timestamp);

    // construct from calendar date
    Julian(int y, unsigned m, unsigned d);

    Julian(const Julian& orig);
    //virtual ~Julian();

    operator double() const {
        return this->julian;
    }

    // assign two Julians
    Julian& operator= (const Julian &other)
    {
        // self-assignment guard
        if (this == &other)
            return *this;
        // do the copy
        julian = other.julian;
        yeardate = other.yeardate;
        // return the existing object so we can chain this operator
        return *this;
    }

    // add constant to julian number
    Julian operator+ (const int days) const {
        return Julian(this->julian + days);
    }

    // tuple of year, month, day
    std::tuple<int, unsigned, unsigned> yeardate;
    // day of week 0 ... 6 monday ... sunday
    int weekday = -1;
    // tuple of hours, minutes, seconds
    std::tuple<unsigned, unsigned, unsigned> daytime;

    // compute calendar date from julian number
    std::tuple<int, unsigned, unsigned> julianDate(double julian);
    // compute time of day from julian number
    std::tuple<unsigned, unsigned, unsigned> timeDay(double julian);
    // output date date portion of year as human readable string, long version
    Glib::ustring formatDateLong();
    // output date portion of julian number as human readable string, short version
    Glib::ustring formatDateShort();
    // output time portion of julian number as human readable string, short version
    Glib::ustring formatTimeShort();

private:
    // is calendar date already initialized
    bool touched = false;
    double julian;
};

#endif /* JULIAN_H */


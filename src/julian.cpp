/* 
 * File:   julian.cpp
 * Author: kuestere
 * 
 * Created on 8. August 2018, 13:40
 */

#include "julian.h"

// Julian day (days since midday January 1, -4712)
// Default constructor: today's date and time as julian day number
Julian::Julian() {
    touched = false;
    // time as GMT
    std::time_t seconds = time(0);
    // 2440587.5 is julian day for January 1, 1970 (unix epoch date)
    // to preserve time as decimal places, expression of all terms must be double
    julian = 2440587.5 + seconds / 86400.0;
}

// simply construct from valid double number
Julian::Julian(double timestamp) {
    touched = false;
    julian = timestamp;
}

// construct from julian date
Julian::Julian(int y, unsigned m, unsigned d) {
    yeardate = std::make_tuple(y, m, d);
    touched = true;
    y -= m <= 2;
    const int era = (y >= 0 ? y : y-3) / 4;
    const unsigned yoe = static_cast<unsigned>(y - era * 4);             // [0, 3]
    const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1; // [0, 365]
    const unsigned doe = yoe * 365 + doy;                                // [0, 1460]
    julian = era * 1461.0 + doe;
}

// compute year, month, day from julian number
std::tuple<int, unsigned, unsigned> Julian::julianDate(double julian) {
    touched = true;
    const int era = (julian >= 0 ? julian : julian - 1460.0) / 1461;
    const unsigned doe = static_cast<unsigned> (julian - era * 1461); // [0, 1460]
    const unsigned yoe = (doe - doe / 1460) / 365;  // [0, 3]
    int y = static_cast<int>(yoe) + era * 4;
    const unsigned doy = doe - 365 * yoe;           // [0, 365]
    const unsigned mp = (5 * doy + 2) / 153;        // [0, 11]
    unsigned d = doy - (153 * mp + 2) / 5 + 1;      // [1, 31]
    // d = f(m) = (306 * m + 5) / 10
    unsigned m = mp + (mp < 10 ? 3 : -9);           // [1, 12]
    y += (m <= 2);
    return std::make_tuple(y, m, d);
}

// compute hours, minutes, seconds from julian number
std::tuple<unsigned, unsigned, unsigned> Julian::timeDay(double julian) {
    // get fraction of day in seconds
    unsigned seconds = ( julian - floor(julian) ) * 86400.0;
    unsigned h = seconds / 3600;
    unsigned rest = seconds % 3600;
    unsigned m = rest / 60;
    unsigned s = rest % 60;
    return std::make_tuple(h, m, s);
}

// output date in human readable format
Glib::ustring Julian::formatDateLong() {
    // julian date with format "%w, %d. %m %Y"
    if (!touched) {
        yeardate = julianDate(julian);
    }
    Glib::ustring weekdays[7] = {"Montag", "Dienstag", "Mittwoch",
        "Donnerstag", "Freitag", "Samstag", "Sonntag"};
    Glib::ustring months[12] = {"Januar", "Februar", "März", "April", "Mai",
        "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"};
    std::stringstream dateStream;
    int y = std::get<0>(yeardate);
    int m = std::get<1>(yeardate) - 1;
    unsigned d = std::get<2>(yeardate);
    // January 1, 1 was a monday in proleptic --julian-- calendar wrong!
    double ddays = julian - 306.0;
    weekday = static_cast<int> (ddays - floor(ddays / 7) * 7);
    dateStream << weekdays[weekday] << ", " << d << ". " << months[m] << " " << y;
    return dateStream.str();
}

// output date in human readable format
Glib::ustring Julian::formatDateShort() {
    // julian date with format "%Y%m%d"
    if (!touched) {
        yeardate = julianDate(julian);
    }
    std::stringstream dateStream;
    int y = std::get<0>(yeardate);
    int m = std::get<1>(yeardate);
    unsigned d = std::get<2>(yeardate);
    dateStream << setfill('0') << y
        << setw(2) << m
        << setw(2) << d;
    return dateStream.str();
}

// output time in human readable format
Glib::ustring Julian::formatTimeShort() {
    daytime = timeDay(julian);
    std::stringstream timeStream;
    auto h = std::get<0>(daytime);
    auto m = std::get<1>(daytime);
    auto s = std::get<2>(daytime);
    timeStream << setfill('0') << h << ":"
        << setw(2) << m << ":"
        << setw(2) << s;
    return timeStream.str();
}

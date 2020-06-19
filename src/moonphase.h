//
//  Moonphase.h
//  Lunar calendar
//
//  Created by Dr. Erich Küster on April 15, 2012
//  C++ on 2. Juni 2018, 14:21
//  Copyright (c) 2012 47809 Krefeld. All rights reserved.
//

#ifndef MOONPHASE_H
#define MOONPHASE_H

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <glibmm/i18n.h>
#include <gtkmm.h>
#include <iomanip>
#include <iostream>
#include <limits>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class MoonPhase
{
public:
    int lunation;
    int phase;
    // as julian day
    double eventTime = 0.0;
    Glib::ustring eventName = "";

    MoonPhase(int luna, int ph) {
        eventName = _(event_names[ph]);
        lunation = luna;
        phase = ph;
    };

    void computeEventTimeAfterJulianDay(double julian);
    double correctMoon(double koeffs[], double E, double F, double M, double MStrich, double Omega);
    double correctQuarter(double E, double F, double M, double MStrich, double Omega, int sign);

private:
    std::vector<const char*> event_names = {
        N_("new moon"),
        N_("crescent moon)"),
        N_("(full moon)"),
        N_("(waning moon"),
        N_("Error: Phase not between 0 and 3")
    };

};

#endif /* MOONPHASE_H */


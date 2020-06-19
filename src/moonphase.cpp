//
//  Moonphase.cpp
//  Lunar calendar
//
//  Created by Dr. Erich Küster on April 15, 2012
//  rewritten for C++ on June 2, 2018
//  Copyright (c) 2012-2018 47809 Krefeld. All rights reserved.
//

#include "moonphase.h"

double MoonPhase::correctMoon(double koeffs[], double E, double F, double M, double MStrich, double Omega)
{
    // Corrections for new resp. full moon
    double add;
    add = koeffs[0] * sin(MStrich);
    add = add + koeffs[1] * E * sin(M);
    add = add + koeffs[2] * sin(2 * MStrich);
    add = add + koeffs[3] * sin(2 * F);
    add = add + koeffs[4] * E * sin(MStrich - M);
    add = add + koeffs[5] * E * sin(MStrich + M);
    add = add + koeffs[6] * E * E * sin(2 * M);
    add = add + koeffs[7] * sin(MStrich - 2 * F);
    add = add + koeffs[8] * sin(MStrich + 2 * F);
    add = add + koeffs[9] * E * sin(2 * MStrich + M);
    add = add + koeffs[10] * sin(3 * MStrich);
    add = add + koeffs[11] * E * sin(M + 2 * F);
    add = add + koeffs[12] * E * sin(M - 2 * F);
    add = add + koeffs[13] * E * sin(2 * MStrich - M);
    add = add + koeffs[14] * sin(Omega);
    add = add + koeffs[15] * sin(MStrich + 2 * M);
    add = add + koeffs[16] * sin(2 * MStrich - 2 * F);
    add = add + koeffs[17] * sin(3 * M);
    add = add + koeffs[18] * sin(MStrich + M - 2 * F);
    add = add + koeffs[19] * sin(2 * MStrich + 2 * F);
    add = add + koeffs[20] * sin(MStrich + M + 2 * F);
    add = add + koeffs[21] * sin(MStrich - M + 2 * F);
    add = add + koeffs[22] * sin(MStrich - M - 2 * F);
    add = add + koeffs[23] * sin(3 * MStrich + M);
    add = add + koeffs[24] * sin(4 * MStrich);
    return add;
}

double MoonPhase::correctQuarter(double E, double F, double M, double MStrich, double Omega, int sign)
{
    // Corrections for first resp. last moon quarter
    double add, W;
    double koeffs[25] = {
        -0.62801, 0.17172, -0.01183, 0.00862, 0.00804, 0.00454, 0.00204, -0.0018,
        -0.0007, -0.0004, -0.00034, 0.00032, 0.00032, -0.00028, 0.00027, -0.00017,
        -5e-05, 4e-05, -4e-05, 4e-05, 3e-05, 3e-05, 2e-05, 2e-05, -2e-05
    };

    add = koeffs[0] * sin(MStrich);
    add = add + koeffs[1] * E * sin(M);
    add = add + koeffs[2] * E * sin(MStrich + M);
    add = add + koeffs[3] * sin(2 * MStrich);
    add = add + koeffs[4] * sin(2 * F);
    add = add + koeffs[5] * E * sin(MStrich - M);
    add = add + koeffs[6] * E * E * sin(2 * M);
    add = add + koeffs[7] * sin(MStrich - 2 * F);
    add = add + koeffs[8] * sin(MStrich + 2 * F);
    add = add + koeffs[9] * sin(3 * MStrich);
    add = add + koeffs[10] * E * sin(2 * MStrich - M);
    add = add + koeffs[11] * E * sin(M + 2 * F);
    add = add + koeffs[12] * E * sin(M - 2 * F);
    add = add + koeffs[13] * E * E * sin(MStrich + 2 * M);
    add = add + koeffs[14] * E * sin(2 * MStrich + M);
    add = add + koeffs[15] * sin(Omega);
    add = add + koeffs[16] * sin(MStrich - M - 2 * F);
    add = add + koeffs[17] * sin(2 * MStrich + 2 * F);
    add = add + koeffs[18] * sin(MStrich + M + 2 * F);
    add = add + koeffs[19] * sin(MStrich - 2 * M);
    add = add + koeffs[20] * sin(MStrich + M - 2 * F);
    add = add + koeffs[21] * sin(3 * M);
    add = add + koeffs[22] * sin(2 * MStrich - 2 * F);
    add = add + koeffs[23] * sin(MStrich - M + 2 * F);
    add = add + koeffs[24] * sin(3 * MStrich + M);
    W = 0.00306 - 0.00038 * E * cos(M) + 0.00026 * cos(MStrich)
            - 2e-05 * cos(MStrich - M) + 2e-05 * cos(MStrich + M) + 2e-05 * cos(2 * F);
    add = add + sign * W;
    return add;
}

void MoonPhase::computeEventTimeAfterJulianDay(double julian) {
    // next lunar phase after julian day
    // see Meeus page 348 ff.
    const double rads = 0.0174532925199433;
    enum { Neumond, Erstes4tel, Vollmond, Letztes4tel };
    double JDE, E, F, k, k0, M, MStrich, Omega, planets, sun, T, T2, T3, T4;

    double nmKoeffListe[] = {
        -0.4072, 0.17241, 0.01608, 0.01039, 0.00739, -0.00514, 0.00208, -0.00111,
        -0.00057, 0.00056, -0.00042, -0.00042, 0.00038, -0.00024, -0.00017, -7e-05,
         4e-05, 4e-05, 3e-05, 3e-05, -3e-05, 3e-05, -2e-05, -2e-05, 2e-05
    };
    double vmKoeffListe[] = {
        -0.40614, 0.17302, 0.01614, 0.01043, 0.00734, -0.00515, 0.00209, -0.00111,
        -0.00057, 0.00056, -0.00042, 0.00042, 0.00038, -0.00024, -0.00017, -7e-05,
         4e-05, 4e-05, 3e-05, 3e-05, -3e-05, 3e-05, -2e-05, -2e-05, 2e-05
    };

    // Coefficients of planets arguments A1 - A14
    double moonPhaseExtras[14][3] = { 
        {299.77,  0.107408, 0.000325},
        {251.88,  0.016321, 0.000165},
        {251.83, 26.651886, 0.000164},
        {349.42, 36.412478, 0.000126},
        { 84.66, 18.206239, 0.000110},
        {141.74, 53.303771, 0.000062},
        {207.14,  2.153732, 0.000060},
        {154.84,  7.306860, 0.000056},
        { 34.52, 27.261239, 0.000047},
        {207.19,  0.121824, 0.000042},
        {291.34,  1.844379, 0.000040},
        {161.72, 24.198154, 0.000037},
        {239.56, 25.513099, 0.000035},
        {331.55,  3.592518, 0.000023}
    };

    // k = (julian + 1 - 2451550.09765) / 29.530588853;
    k = (julian - 2451550.09765) / 29.530588853;
    k0 = floor(k) + lunation + 0.25 * phase;
    T = k0 / 1236.85;
    T2 = T * T;
    T3 = T2 * T;
    T4 = T2 * T2;
    E = 1 - 0.002516 * T - 7.4e-06 * T2;
    // mean anomaly of sun at time JDE
    M = (2.5534 + 29.10535669 * k0 - 2.18e-05 * T2 - 1.1e-07 * T3) * rads;
    // mean anomaly of moon at time JDE
    MStrich = (201.5643 + 385.81693528 * k0 + 0.1017438 * T2 + 1.239e-05 * T3 - 5.8e-08 * T4) * rads;
    // Argument der Breite des Mondes
    F = (160.7108 + 390.67050274 * k0 - 0.0016341 * T2 - 2.27e-06 * T3 + 1.1e-08 * T4) * rads;
    // Longitude of the ascending lunar node LOAN
    Omega = (124.7746 - 1.5637558 * k0 + 0.0020691 * T2 + 2.15e-06 * T3) * rads;
    JDE = 2451550.09765 + 29.530588853 * k0 + 0.0001337 * T2 - 1.5e-07 * T3 + 7.3e-10 * T4;
    // apply perturbation terms due to sun and planets
    // first the sun
    sun = 0.0;
    switch (phase)
    {
        case Neumond:
            sun = MoonPhase::correctMoon(nmKoeffListe, E, F, M, MStrich, Omega);
            break;

        case Erstes4tel:
            sun = MoonPhase::correctQuarter(E, F, M, MStrich, Omega, 1);
            break;
            
        case Vollmond:
            sun = MoonPhase::correctMoon(vmKoeffListe, E, F, M, MStrich, Omega);
            break;
            
        case Letztes4tel:
            sun = MoonPhase::correctQuarter(E, F, M, MStrich, Omega, -1);
            break;
            
        default:
            cout << _("Error: Phase not between 0 and 3") << endl;
            break;
    }
    // extra perturbations due to planets
    planets = 0.0;
    double args[14];
    for (int i = 0; i < 14; ++i) {
        args[i] = (moonPhaseExtras[i][0] + k0 * moonPhaseExtras[i][1]) * rads;
    }
    args[0] -= 0.009173 * T * T * rads;
    for (int i = 0; i < 14; ++i) {
        planets += (moonPhaseExtras[i][2] * sin(args[i]));
    }
    //cout << fixed << "jde: " << JDE << " sun: " << sun << " planets: " << planets << endl;
    eventTime = JDE + sun + planets;
}

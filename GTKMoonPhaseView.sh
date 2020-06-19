#!/bin/bash
cd ./src
g++ `pkg-config gtkmm-3.0 --cflags --libs` -c main.cpp moonphasewindow.cpp moonphase.cpp
g++ main.o moonphase.o moonphasewindow.o -o GTKMoonPhaseView `pkg-config gtkmm-3.0 --cflags --libs`
./GTKMoonPhaseView


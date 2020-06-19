# GTK-Moon-Phase-View
Moon phase calendar with creating of ICS files

See all moon phases of a given year and put the dates into your personal calendar

The program was written in C++ with use of gtkmm wrapper for Gnome GTK environment of Linux.

The program follows the design of my earlier program Moon-Phase-View here on Github written in Swift. Since I switched from macOS to Fedora Linux (for what reasons ever) I had to change my programming language. For "simplicity" I choose C++ and to my pleasure found its "way to think" very similar to Swift and Objective-C. In some aspects I found C++ easier compared to Swift ...r

I wrote this program to become familiar with the C++ language, especially the GTK-API under the gtkmm wrapper and to get a feeling how to display liststore treeviews on the screen. Take it as example for handling of windows, calendars and dates.

The normal procedure for building the program is using the standard autotools. The following sequence
```
./autogen.sh --prefix=$HOME/.local
make
make install
moonphaseview.sh
```
will build and run the program for the current user. The Linux way of localization for german translation file is adapted.

Alternatively you can easily compile and run the program in Terminal with the added shell script:
```
sh ./GTKMoonPhaseView.sh
```
Usage: You will find the program mostly self explaining. On input of a year number greater than 1970 the phases of moon are calculated. By clicking on "Save..." you have the possibilty to save the dates for the phases in a calendar file with the extension .ics, which will accepted by calendar programs on PC or mobile phones. Thereby you are able to view the phases of the moon on your personal calendar and know exactly, when you will be moonstruck ...r

Very useful algorithms for calculating are found in the excellent book "Astronomische Algorithmen" by Jean Meeus, Johann Ambrosius Barth 1992, ISBN-13 978-3335003182. Unfortunately the book is not any longer available.

Every lunar cycle ( called lunation ) consists of four phases

- new moon, no moon visible
- crescent moon, half of the moon is visible, the sickle is on the right side smooth )
- full moon, all of the moon is visible ( )
- waning moon, half of the moon is visible, the sickle is on the left side smooth (

In most cases the dates for the lunar events are accurate to roundabout some seconds.


Disclaimer: Use the program for what purpose you like, but hold in mind, that I will not be responsible for any harm it will cause to your hard- or software. It was your decision to use this piece of software.


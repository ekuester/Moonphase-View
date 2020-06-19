/*
 * File:   main.cpp
 * Author: kuestere
 *
 * Created on 29. Mai 2018, 16:52
 *  switch to autotools build system on 11. Juni 2020, 13:08
 *  works with gnome-builder, too
 */

#include "moonphasewindow.h"
#include <gtkmm/application.h>

#ifndef NLS
#define NLS 1
#define PROGRAMNAME_LOCALEDIR "."
#define GETTEXT_PACKAGE "GTKMoonPhaseView"
#endif

int main(int argc, char *argv[])
{
#ifdef NLS
	setlocale(LC_ALL, "");
	bindtextdomain(GETTEXT_PACKAGE, PROGRAMNAME_LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);
#endif

    auto app = Gtk::Application::create(argc, argv, "org.kuester.moonphaseview");

    MoonPhaseWindow window;

    //Shows the window and returns when it is closed.
    return app->run(window);
}

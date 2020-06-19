ACLOCAL_AMFLAGS = -I m4

installdir = $(datadir)/$(PACKAGE)
install_PROGRAMS = GTKMoonPhaseView

GTKMoonPhaseView_SOURCES = \
julian.h julian.cpp \
main.cpp \
moonphase.h moonphase.cpp \
moonphasewindow.h moonphasewindow.cpp

GTKMoonPhaseView_CPPFLAGS = $(GTKMM_CFLAGS) $(CAIRO_CFLAGS) \
-DPROGRAMNAME_LOCALEDIR=\"$(PROGRAMNAME_LOCALEDIR)\" \
-DGETTEXT_PACKAGE=\"${GETTEXT_PACKAGE}\" \
-DNLS=1

GTKMoonPhaseView_LDADD = $(GTKMM_LIBS) $(CAIRO_LIBS)


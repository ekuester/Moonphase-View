SUBDIRS = src po
#SUBDIRS = src po 16x16

ACLOCAL_AMFLAGS = -I m4

bin_SCRIPTS = moonphaseview.sh

#LATERAL_FILES = COMMENTAREN COMMENTAIRES COMMENTI COMMENTS KOMMENTARE LICENSE README.md
#LEGEND_FILES = legend-DE-480x140.svg\
# legend-EN-480x140.svg\
# legend-FR-480x140.svg\
# legend-IT-480x140.svg\
# legend-NL-480x140.svg
#lateraldir = $(pkgdatadir)
#lateral_DATA = $(LATERAL_FILES) $(LEGEND_FILES)

#EXTRA_DIST = 16x16 autogen.sh GTKMoonPhaseView.sh $(LATERAL_FILES) $(LEGEND_FILES)
EXTRA_DIST = autogen.sh GTKMoonPhaseView.sh

CLEANFILES =

# desktop files
desktopdir = $(datadir)/applications
#desktop_DATA = moonphaseview.desktop

# convenience command for doing Makefile variable substitutions in non-Makefile
# files (scripts, service files, etc.)
do_subst = sed -e 's|@abs_top_srcdir[@]|$(abs_top_srcdir)|g' \
	-e 's|@abs_top_builddir[@]|$(abs_top_builddir)|g' \
	-e 's|@localedir[@]|$(localedir)|g' \
	-e 's|@bindir[@]|$(bindir)|g' \
	-e 's|@libexecdir[@]|$(libexecdir)|g' \
	-e 's|@pkglibdir[@]|$(pkglibdir)|g' \
	-e 's|@pkgdatadir[@]|$(pkgdatadir)|g' \
	-e 's|@have_libnotify[@]|$(HAVE_LIBNOTIFY)|g' \
	-e 's|@have_libsoup[@]|$(HAVE_LIBSOUP)|g' \
	-e 's|@have_cheese[@]|$(HAVE_CHEESE)|g'

moonphaseview.sh: moonphaseview.sh.in
	$(AM_V_GEN) $(do_subst) $< > $@
	chmod +x $@

CLEANFILES += moonphaseview.sh
EXTRA_DIST += moonphaseview.sh.in

# application icon 128x128
appicondir=$(datadir)/pixmaps
#appicon_DATA=moonphaseview.svg

# EXTRA_DIST += moonphaseview.svg


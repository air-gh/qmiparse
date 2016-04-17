
SUBDIRS = . test

bin_PROGRAMS = qmicli qmiparse

qmicli_CPPFLAGS = \
	$(GLIB_CFLAGS) \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src/libqmi-glib \
	-I$(top_srcdir)/src/libqmi-glib/generated \
	-I$(top_builddir)/src/libqmi-glib \
	-I$(top_builddir)/src/libqmi-glib/generated

qmiparse_CPPFLAGS = \
	$(GLIB_CFLAGS) \
	-I$(top_srcdir) \
	-I$(top_srcdir)/src/libqmi-glib \
	-I$(top_srcdir)/src/libqmi-glib/generated \
	-I$(top_builddir)/src/libqmi-glib \
	-I$(top_builddir)/src/libqmi-glib/generated

qmicli_SOURCES = \
	qmicli.c \
	qmicli.h \
	qmicli-helpers.c \
	qmicli-helpers.h \
	qmicli-dms.c \
	qmicli-wds.c \
	qmicli-nas.c \
	qmicli-pbm.c \
	qmicli-uim.c \
	qmicli-wms.c \
	qmicli-wda.c \
	qmicli-voice.c

qmiparse_SOURCES = \
	qmiparse.c

qmicli_LDADD = \
	$(GLIB_LIBS) \
	$(top_builddir)/src/libqmi-glib/libqmi-glib.la

qmiparse_LDADD = \
	$(GLIB_LIBS) \
	$(top_builddir)/src/libqmi-glib/libqmi-glib.la


completiondir = $(datadir)/bash-completion/completions

install-data-hook:
	$(mkinstalldirs) $(DESTDIR)$(completiondir)
	$(INSTALL_DATA) $(srcdir)/qmicli-completion $(DESTDIR)$(completiondir)/qmicli $(DESTDIR)$(completiondir)/qmiparse

uninstall-hook:
	rm -f $(DESTDIR)$(completiondir)/qmicli $(DESTDIR)$(completiondir)/qmiparse

EXTRA_DIST = qmicli-completion

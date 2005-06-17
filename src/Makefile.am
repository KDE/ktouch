# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to.
# kde_icondir       Where your icon should go to.
# kde_sounddir      Where system sounds should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# claim, which subdirectories you want to install
# if you don't have subdirectories, comment it
# SUBDIRS =

# these are the headers for your project
noinst_HEADERS = ktouch.h ktouchkeyconnector.h ktouchkeys.h ktouchlecture.h \
	ktouchleveldata.h ktouchslideline.h ktouchstatistics.h ktouchstatus.h ktouchtrainer.h \
	ktouchkeyboardwidget.h ktouchkeyboard.h ktouchkey.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/ktouch.pot


#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = ktouch

# the application source, library search path, and link libraries
ktouch_SOURCES = ktouch.cpp ktouchkeys.cpp ktouchlecture.cpp \
		ktouchleveldata.cpp ktouchslideline.cpp ktouchstatistics.cpp ktouchstatus.cpp \
		ktouchtrainer.cpp main.cpp ktouchprefcolorslayout.ui ktouchprefgenerallayout.ui ktoucheditor_dlg.ui \
		ktouchprefkeyboardlayout.ui ktouchpreftraininglayout.ui ktouchlectureeditor_dlg.ui  ktouchlectureeditor.cpp \
		ktouchopenrequest.cpp ktouchopenrequest_dlg.ui ktouchcolorscheme.cpp ktouchutils.cpp \
		prefs.kcfgc ktouchkeyboardwidget.cpp ktouchkeyboardeditor_dlg.ui \
		ktouchkeyboardeditor.cpp ktouchkeyboard.cpp ktouchkey.cpp \
		ktouchkeyconnector.cpp ktouchstatistics_dlg.ui ktouchstatuslayout.ui ktouchstatisticsdata.cpp \
		ktouchdefaults.cpp ktouchchartwidget.cpp

# the library search path.
ktouch_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against. Be aware of the order. First the libraries,
# that depend on the following ones.
ktouch_LDADD = -lkdeeduplot $(LIB_KIO)

rcdir = $(kde_datadir)/ktouch
rc_DATA = ktouchui.rc
kde_kcfg_DATA = ktouch.kcfg



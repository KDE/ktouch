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
noinst_HEADERS = ktouch.h ktouchprefgeneral.h ktouchprefkeyboard.h ktouchkeyboardcolor.h ktouchtrainingsession.h ktouchpreftraining.h ktouchpref.h ktouchkeyconnector.h ktouchtrainer.h ktouchstatistics.h ktouchkeys.h ktoucheditor.h ktouchslideline.h ktouchstatus.h ktouchlecture.h ktouchleveldata.h

# let automoc handle all of the meta source files (moc)
ktouch_METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/ktouch.pot


#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = ktouch

# the application source, library search path, and link libraries
ktouch_SOURCES = main.cpp ktouch.cpp ktouchprefgeneral.cpp ktouchprefkeyboard.cpp ktouchpreftraining.cpp ktouchpref.cpp  ktouchstatus.cpp ktouchlecture.cpp ktouchleveldata.cpp ktoucheditor.cpp  ktouchslideline.cpp ktouchkeys.cpp ktouchkeyboard.cpp ktouchsettings.cpp ktouchtrainer.cpp ktouchtrainingsession.cpp  ktouchcharstats.cpp ui/ktouchstatisticslayout.ui ktouchstatistics.cpp ui/ktouchstatuslayout.ui ui/ktouchprefgenerallayout.ui ui/ktouchprefkeyboardlayout.ui ui/ktouchpreftraininglayout.ui ui/ktoucheditorlayout.ui ui/ktouchstartnewdialog.ui

# the library search path.
ktouch_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against. Be aware of the order. First the libraries,
# that depend on the following ones.
ktouch_LDADD = $(LIB_KIO) $(LIB_KDEUI)

rcdir = $(kde_datadir)/ktouch
rc_DATA = ktouchui.rc


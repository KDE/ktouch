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
INCLUDES= $(all_includes)

# claim, which subdirectories you want to install
# if you don't have subdirectories, comment it
# SUBDIRS = 

####### This part is very ktouch specific
# you can add here more. This one gets installed
bin_PROGRAMS = 	ktouch

# Which sources should be compiled for ktouch.
ktouch_SOURCES = optionGeneralDlg.ui \
optionKeyboardDlg.ui \
optionTrainingDlg.ui \
touchLineLayout.ui \
touchStatusLayout.ui \
ktouch.cpp \
touchKeyboard.cpp \
touchStatus.cpp \
touchkey.cpp \
touchlecture.cpp \
optionKeyboard.cpp \
slideLine.cpp \
touchline.cpp \
controlkey.cpp \
fingerkey.cpp \
hiddenkey.cpp \
normalkey.cpp \
optionGeneral.cpp \
optionTraining.cpp \
main.cpp \
touchstat.cpp

ktouch_METASOURCES = AUTO

# the library search path.
ktouch_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# the libraries to link against. Be aware of the order. First the libraries,
# that depend on the following ones.
ktouch_LDADD   = $(LIB_KFILE)

# this option you can leave out. Just, if you use "make dist", you need it
#noinst_HEADERS = ktouch.h

rcdir = $(kde_datadir)/ktouch
rc_DATA = ktouchui.rc

# the -x is for skipping messages already translated in kdelibs
messages: rc.cpp
	$(XGETTEXT) rc.cpp *.cpp -o $(podir)/ktouch.pot



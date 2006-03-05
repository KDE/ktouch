kde4_header()

include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/kdeeduplot/ ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(ktouch_SRCS 
   ktouchchartwidget.cpp 
   ktouchcolorscheme.cpp 
   ktouch.cpp 
   ktouchdefaults.cpp 
   ktouchkeyboard.cpp 
   ktouchkeyboardeditor.cpp 
   ktouchkeyboardwidget.cpp 
   ktouchkeyconnector.cpp 
   ktouchkey.cpp 
   ktouchkeys.cpp 
   ktouchlecture.cpp 
   ktouchlectureeditor.cpp 
   ktouchleveldata.cpp 
   ktouchopenrequest.cpp 
   ktouchslideline.cpp 
   ktouchstatistics.cpp 
   ktouchstatisticsdata.cpp 
   ktouchstatus.cpp 
   ktouchtrainer.cpp 
   ktouchutils.cpp 
   main.cpp )

kde4_automoc(${ktouch_SRCS})

kde4_add_ui3_files(ktouch_SRCS 
   ktouchkeyboardeditor_dlg.ui 
   ktouchlectureeditor_dlg.ui 
   ktouchopenrequest_dlg.ui 
   ktouchprefcolorslayout.ui 
   ktouchprefgenerallayout.ui 
   ktouchprefkeyboardlayout.ui 
   ktouchpreftraininglayout.ui 
   ktouchstatistics_dlg.ui 
   ktouchstatuslayout.ui )

kde4_add_kcfg_files(ktouch_SRCS prefs.kcfgc )

kde4_add_executable(ktouch ${ktouch_SRCS})

target_link_libraries(ktouch  ${QT_AND_KDECORE_LIBS} kdeeduplot kio )

install_targets(/bin ktouch )


########### install files ###############

install_files( ${KCFG_INSTALL_DIR} FILES ktouch.kcfg )
install_files( ${DATA_INSTALL_DIR}/ktouch FILES ktouchui.rc )

kde4_footer()



#original Makefile.am contents follow:

## this 10 paths are KDE specific. Use them:
## kde_htmldir       Where your docs should go to. (contains lang subdirs)
## kde_appsdir       Where your application file (.kdelnk) should go to.
## kde_icondir       Where your icon should go to.
## kde_sounddir      Where system sounds should go to.
## kde_datadir       Where you install application data. (Use a subdir)
## kde_locale        Where translation files should go to.(contains lang subdirs)
## kde_cgidir        Where cgi-bin executables should go to.
## kde_confdir       Where config files should go to.
## kde_mimedir       Where mimetypes should go to.
## kde_toolbardir    Where general toolbar icons should go to.
## kde_wallpaperdir  Where general wallpapers should go to.
#
## set the include path for X, qt and KDE
#INCLUDES = -I$(top_srcdir)/libkdeedu/kdeeduplot/ -I$(top_srcdir) $(all_includes) 
#
## claim, which subdirectories you want to install
## if you don't have subdirectories, comment it
## SUBDIRS =
#
## these are the headers for your project
#noinst_HEADERS = \
#	ktouchchartwidget.h \
#	ktouchcolorscheme.h \
#	ktouchdefaults.h \
#	ktouch.h \
#	ktouchkeyboardeditor.h \
#	ktouchkeyboard.h \
#	ktouchkeyboardwidget.h \
#	ktouchkeyconnector.h \
#	ktouchkey.h \
#	ktouchkeys.h \
#	ktouchlectureeditor.h \
#	ktouchlecture.h \
#	ktouchleveldata.h \
#	ktouchopenrequest.h \
#	ktouchslideline.h \
#	ktouchstatisticsdata.h \
#	ktouchstatistics.h \
#	ktouchstatus.h \
#	ktouchtrainer.h \
#	ktouchutils.h
#
## let automoc handle all of the meta source files (moc)
#ktouch_METASOURCES = AUTO
#
#messages: rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/ktouch.pot
#
#
##########################################################################
## APPLICATION SECTION
##########################################################################
## this is the program that gets installed.  it's name is used for all
## of the other Makefile.am variables
#bin_PROGRAMS = ktouch
#
## the application source, library search path, and link libraries
#ktouch_SOURCES = \
#	ktouchchartwidget.cpp \
#	ktouchcolorscheme.cpp \
#	ktouch.cpp \
#	ktouchdefaults.cpp \
#	ktouchkeyboard.cpp \
#	ktouchkeyboardeditor.cpp \
#	ktouchkeyboardeditor_dlg.ui \
#	ktouchkeyboardwidget.cpp \
#	ktouchkeyconnector.cpp \
#	ktouchkey.cpp \
#	ktouchkeys.cpp \
#	ktouchlecture.cpp \
#	ktouchlectureeditor.cpp \
#	ktouchlectureeditor_dlg.ui \
#	ktouchleveldata.cpp \
#	ktouchopenrequest.cpp \
#	ktouchopenrequest_dlg.ui \
#	ktouchprefcolorslayout.ui \
#	ktouchprefgenerallayout.ui \
#	ktouchprefkeyboardlayout.ui \
#	ktouchpreftraininglayout.ui \
#	ktouchslideline.cpp \
#	ktouchstatistics.cpp \
#	ktouchstatisticsdata.cpp \
#	ktouchstatistics_dlg.ui \
#	ktouchstatus.cpp \
#	ktouchstatuslayout.ui \
#	ktouchtrainer.cpp \
#	ktouchutils.cpp \
#	main.cpp \
#	prefs.kcfgc
#
## the library search path.
#ktouch_LDFLAGS = $(KDE_RPATH) $(all_libraries)
#
## the libraries to link against. Be aware of the order. First the libraries,
## that depend on the following ones.
#ktouch_LDADD = ../../libkdeedu/kdeeduplot/libkdeeduplot.la $(LIB_KIO)
#
#rcdir = $(kde_datadir)/ktouch
#rc_DATA = ktouchui.rc
#kde_kcfg_DATA = ktouch.kcfg
#

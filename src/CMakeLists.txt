# CMakeList.txt for the KTouch source directory

# set include directories
#include_directories( )

# add ui files, all files in directory ui are Qt4 only
kde4_add_ui_files(ktouch_UIS
   ui/ktouchcoloreditordialog.ui 
   ui/ktouchopenrequestdialog.ui 
   ui/ktouchstatuswidget.ui
   ui/ktouchlectureeditordialog.ui 
   ui/ktouchstatisticsdialog.ui 
   ui/ktouchusersetupdialog.ui
   ui/ktouchkeyboardeditordialog.ui
   ui/ktouchprefgeneralpage.ui 
   ui/ktouchprefcolorspage.ui 
   ui/ktouchprefkeyboardpage.ui 
   ui/ktouchpreftrainingpage.ui
   ui/ktouchprefsoundpage.ui
)

# set the source code files from which KTouch is compiled
set(ktouch_SRCS
   ktouchcoloreditordialog.cpp 
   ktouchopenrequestdialog.cpp 
   ktouchlectureeditordialog.cpp 
   ktouchkeyboardeditordialog.cpp 
   ktouchchartwidget.cpp 
   ktouchcolorscheme.cpp 
   ktouchdefaults.cpp 
   ktouchkeyboard.cpp
   ktouchkeyboardwidget.cpp 
   ktouchkeyconnector.cpp 
   ktouchkey.cpp 
   ktouchkeys.cpp 
   ktouchlecture.cpp 
   ktouchleveldata.cpp 
   ktouchtextlinewidget.cpp 
   ktouchstatisticsdialog.cpp 
   ktouchstatisticsdata.cpp 
   ktouchstatuswidget.cpp 
   ktouchtrainer.cpp
   ktouchusersetupdialog.cpp 
   ktouchutils.cpp 
   ktouch.cpp
   main.cpp
)

# create moc files automatically, requires that moc files are included into the cpp files
kde4_automoc( ${ktouch_UIS} ${ktouch_SRCS} )

kde4_add_kcfg_files(ktouch_SRCS prefs.kcfgc )

kde4_add_executable(ktouch ${ktouch_UIS} ${ktouch_SRCS})

target_link_libraries(ktouch  ${KDE4_KIO_LIBS} ${KDE4_PHONON_LIBS})

install(TARGETS ktouch  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES ktouch.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES ktouchui.rc  DESTINATION  ${DATA_INSTALL_DIR}/ktouch )



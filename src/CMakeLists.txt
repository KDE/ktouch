# CMakeList.txt for the KTouch source directory

# set include directories
#include_directories( )

# add ui files, all files in directory ui are Qt4 only
kde4_add_ui_files(ktouch_UIS
   ui/ktouchcoloreditordialog.ui 
   ui/ktouchopenrequestdialog.ui 
   ui/ktouchstatuswidget.ui
   ui/ktouchlectureeditordialog.ui 
   ui/ktouchlevelsummarydialog.ui
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
   ktouchcolorscheme.cpp 
   ktouchdefaults.cpp 
   ktouchkeyboard.cpp
   ktouchkeyboardwidget.cpp 
   ktouchkeyconnector.cpp 
   ktouchkey.cpp 
   ktouchlecture.cpp 
   ktouchleveldata.cpp 
   ktouchlevelsummarydialog.cpp
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

kde4_add_kcfg_files(ktouch_SRCS prefs.kcfgc )

#uncomment this if oxygen icons for ktouch are available
#set(ktouch_icons
#    ${CMAKE_INSTALL_PREFIX}/share/icons/oxygen/16x16/apps/ktouch.png
#    ${CMAKE_INSTALL_PREFIX}/share/icons/oxygen/32x32/apps/ktouch.png
#    ${CMAKE_INSTALL_PREFIX}/share/icons/oxygen/48x48/apps/ktouch.png
#    ${CMAKE_INSTALL_PREFIX}/share/icons/oxygen/64x64/apps/ktouch.png
#    ${CMAKE_INSTALL_PREFIX}/share/icons/oxygen/128x128/apps/ktouch.png
#)
set(ktouch_icons
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi16-app-ktouch.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi32-app-ktouch.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi48-app-ktouch.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi128-app-ktouch.png
)
kde4_add_win32_app_icon(ktouch_SRCS ${ktouch_icons})
kde4_add_executable(ktouch ${ktouch_UIS} ${ktouch_SRCS})

target_link_libraries(ktouch  ${KDE4_KIO_LIBS} ${KDE4_PHONON_LIBS})

install(TARGETS ktouch  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES ktouch.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES ktouchui.rc  DESTINATION  ${DATA_INSTALL_DIR}/ktouch )



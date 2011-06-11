# CMakeList.txt for the KTouch source directory

# set include directories
#include_directories( )

# add compilation options
option( KDEEDU_KTOUCH_BUILD_WITH_PHONON "Enables compilation of sound output using Phonon." ON )
mark_as_advanced(KDEEDU_KTOUCH_BUILD_WITH_PHONON)

# configure the local configuration file
configure_file(ktouch_build_config.h.in    ktouch_build_config.h)

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

kde4_add_kcfg_files(ktouch_SRCS prefs.kcfgc )

#uncomment this if oxygen icons for ktouch are available
#kde4_add_app_icon(ktouch_SRCS "${KDE4_ICON_DIR}/oxygen/*/apps/ktouch.png")
kde4_add_app_icon(ktouch_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-ktouch.png")

kde4_add_executable(ktouch ${ktouch_UIS} ${ktouch_SRCS})

target_link_libraries(ktouch ${KDE4_KIO_LIBS} ${QT_QTCORE_LIBRARY})

if (KDEEDU_KTOUCH_BUILD_WITH_PHONON)
    target_link_libraries(ktouch ${KDE4_PHONON_LIBS})
endif (KDEEDU_KTOUCH_BUILD_WITH_PHONON)

install(TARGETS ktouch  ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( FILES ktouch.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES ktouchui.rc  DESTINATION  ${DATA_INSTALL_DIR}/ktouch )



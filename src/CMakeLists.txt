
include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/kdeeduplot/  )


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

kde4_add_ui_files(ktouch_SRCS 
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

target_link_libraries(ktouch  ${KDE4_KIO_LIBS} kdeeduplot ${KDE4_PHONONCORE_LIBRARY})

install(TARGETS ktouch  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES ktouch.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES ktouchui.rc  DESTINATION  ${DATA_INSTALL_DIR}/ktouch )



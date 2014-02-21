# CMakeList.txt for the KTouch source directory

include(MacroLogFeature)

find_package(KDeclarative REQUIRED)

# Currently it is called by FindQt4, but call it explicitly
find_package(X11)
macro_log_feature(X11_Xkbfile_FOUND "libxkbfile" "X Keyboard Extension" "http://www.x.org/wiki/XKB" FALSE "" "Support for keyboard layout auto-detection in KTouch")

add_subdirectory(qml)
add_subdirectory(schemata)

# set include directories
include_directories(${QT_INCLUDE} ${KDE4_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

# add compilation options
option(KDEEDU_KTOUCH_BUILD_WITH_PHONON "Enables compilation of sound output using Phonon." ON)
mark_as_advanced(KDEEDU_KTOUCH_BUILD_WITH_PHONON)

# configure the local configuration file
configure_file(ktouch_build_config.h.in ktouch_build_config.h)

# add ui files, all files in directory ui are Qt4 only
kde4_add_ui_files(ktouch_UIS
    ui/colorsconfigwidget.ui
    ui/trainingconfigwidget.ui
    ui/resourceeditorwidget.ui
    ui/resourcetypeswidget.ui
    ui/newcoursewidget.ui
    ui/newkeyboardlayoutwidget.ui
    ui/resourcetemplatewidget.ui
    ui/courseeditor.ui
    ui/lessontexteditor.ui
    ui/keyboardlayouteditor.ui
    ui/keyboardlayoutpropertieswidget.ui
    ui/customlessoneditorwidget.ui
)

# set the source code files from which KTouch is compiled
set(ktouch_SRCS
    main.cpp
    application.cpp
    mainwindow.cpp
    bindings/utils.cpp
    bindings/stringformatter.cpp
    declarativeitems/applicationbackground.cpp
    declarativeitems/cursorshapearea.cpp
    declarativeitems/griditem.cpp
    declarativeitems/lessonpainter.cpp
    declarativeitems/preferencesproxy.cpp
    declarativeitems/scalebackgrounditem.cpp
    declarativeitems/traininglinecore.cpp
    core/resource.cpp
    core/keyboardlayoutbase.cpp
    core/keyboardlayout.cpp
    core/abstractkey.cpp
    core/key.cpp
    core/keychar.cpp
    core/specialkey.cpp
    core/coursebase.cpp
    core/course.cpp
    core/lesson.cpp
    core/trainingstats.cpp
    core/profile.cpp
    core/dataindex.cpp
    core/dataaccess.cpp
    core/dbaccess.cpp
    core/profiledataaccess.cpp
    core/resourcedataaccess.cpp
    core/userdataaccess.cpp
    undocommands/coursecommands.cpp
    undocommands/keyboardlayoutcommands.cpp
    models/resourcemodel.cpp
    models/lessonmodel.cpp
    models/charactersmodel.cpp
    models/categorizedresourcesortfilterproxymodel.cpp
    models/errorsmodel.cpp
    models/learningprogressmodel.cpp
    editor/resourceeditor.cpp
    editor/resourceeditorwidget.cpp
    editor/newresourceassistant.cpp
    editor/resourcetypeswidget.cpp
    editor/newcoursewidget.cpp
    editor/newkeyboardlayoutwidget.cpp
    editor/resourcetemplatewidget.cpp
    editor/abstracteditor.cpp
    editor/courseeditor.cpp
    editor/keyboardlayoutcombobox.cpp
    editor/lessontexteditor.cpp
    editor/lessontexthighlighter.cpp
    editor/keyboardlayouteditor.cpp
    editor/keyboardlayouteditorview.cpp
    editor/keyboardlayoutpropertieswidget.cpp
    editor/charactersviewdelegate.cpp
    trainingconfigwidget.cpp
    colorsconfigwidget.cpp
    customlessoneditorwidget.cpp
)

if (X11_Xkbfile_FOUND)
    add_definitions(-DKTOUCH_BUILD_WITH_X11)
    include_directories(${X11_Xkbfile_INCLUDE_PATH})
    set(ktouch_SRCS ${ktouch_SRCS} x11_helper.cpp)
else (X11_Xkbfile_FOUND)
    set(ktouch_SRCS ${ktouch_SRCS} keyboardlayoutmenu.cpp)
endif (X11_Xkbfile_FOUND)

kde4_add_kcfg_files(ktouch_SRCS preferences.kcfgc )

#uncomment this if oxygen icons for ktouch are available
#kde4_add_app_icon(ktouch_SRCS "${KDE4_ICON_DIR}/oxygen/*/apps/ktouch.png")
kde4_add_app_icon(ktouch_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-ktouch.png")

kde4_add_executable(ktouch ${ktouch_UIS} ${ktouch_SRCS})

target_link_libraries(ktouch ${KDECLARATIVE_LIBRARIES} ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KCMUTILS_LIBS} ${QT_QTCORE_LIBRARY} ${QT_QTOPENGL_LIBRARY} ${QT_QTSCRIPT_LIBRARY} ${QT_QTDECLARATIVE_LIBRARY} ${QT_QTXMLPATTERNS_LIBRARY} ${QT_QTSQL_LIBRARY})

if (X11_Xkbfile_FOUND)
    target_link_libraries(ktouch ${X11_Xkbfile_LIB} ${X11_LIBRARIES})
endif (X11_Xkbfile_FOUND)

install(TARGETS ktouch ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ktouch.kcfg DESTINATION ${KCFG_INSTALL_DIR})

# CMakeList.txt for the KTouch source directory

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
    ui/keyboardlayouteditor.ui
    ui/keyboardlayoutpropertieswidget.ui
)

# set the source code files from which KTouch is compiled
set(ktouch_SRCS
    main.cpp
    application.cpp
    mainwindow.cpp
    bindings/utils.cpp
    bindings/stringformatter.cpp
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
    core/profiledataaccess.cpp
    models/resourcemodel.cpp
    models/lessonmodel.cpp
    models/categorizedresourcesortfilterproxymodel.cpp
    editor/resourceeditor.cpp
    editor/resourceeditorwidget.cpp
    editor/newresourceassistant.cpp
    editor/resourcetypeswidget.cpp
    editor/newcoursewidget.cpp
    editor/newkeyboardlayoutwidget.cpp
    editor/resourcetemplatewidget.cpp
    editor/abstracteditor.cpp
    editor/courseeditor.cpp
    editor/coursecommands.cpp
    editor/keyboardlayoutcombobox.cpp
    editor/lessontexthighlighter.cpp
    editor/keyboardlayouteditor.cpp
    editor/keyboardlayouteditorview.cpp
    editor/keyboardlayoutpropertieswidget.cpp
    editor/keyboardlayoutcommands.cpp
    editor/griditem.cpp
    scalebackgrounditem.cpp
    x11_helper.cpp
    preferencesproxy.cpp
    trainingconfigwidget.cpp
    colorsconfigwidget.cpp
)

kde4_add_kcfg_files(ktouch_SRCS preferences.kcfgc )

#uncomment this if oxygen icons for ktouch are available
#kde4_add_app_icon(ktouch_SRCS "${KDE4_ICON_DIR}/oxygen/*/apps/ktouch.png")
kde4_add_app_icon(ktouch_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-ktouch.png")

kde4_add_executable(ktouch ${ktouch_UIS} ${ktouch_SRCS})

target_link_libraries(ktouch kdeclarative ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS} ${QT_QTCORE_LIBRARY} ${QT_QTOPENGL_LIBRARY} ${QT_QTSCRIPT_LIBRARY} ${QT_QTDECLARATIVE_LIBRARY} ${QT_QTXMLPATTERNS_LIBRARY} ${QT_QTSQL_LIBRARY})
target_link_libraries(ktouch ${X11_Xkbfile_LIB} ${X11_LIBRARIES})

if (KDEEDU_KTOUCH_BUILD_WITH_PHONON)
    target_link_libraries(ktouch ${KDE4_PHONON_LIBS})
endif (KDEEDU_KTOUCH_BUILD_WITH_PHONON)

install(TARGETS ktouch ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ktouch.kcfg DESTINATION ${KCFG_INSTALL_DIR})

ecm_setup_version(${RELEASE_SERVICE_VERSION}
    VARIABLE_PREFIX KTOUCH
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KTouchConfigVersion.cmake"
)

ecm_optional_add_subdirectory(schemata)

# set include directories
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${ktouch_SOURCE_DIR}
)

# set the source code files from which KTouch is compiled
set(ktouch_SRCS
    application.cpp
    application.h
    bindings/stringformatter.cpp
    bindings/stringformatter.h
    bindings/utils.cpp
    bindings/utils.h
    colorsconfigwidget.cpp
    colorsconfigwidget.h
    core/abstractkey.cpp
    core/abstractkey.h
    core/coursebase.cpp
    core/coursebase.h
    core/course.cpp
    core/course.h
    core/dataaccess.cpp
    core/dataaccess.h
    core/dataindex.cpp
    core/dataindex.h
    core/dbaccess.cpp
    core/dbaccess.h
    core/keyboardlayoutbase.cpp
    core/keyboardlayoutbase.h
    core/keyboardlayout.cpp
    core/keyboardlayout.h
    core/keychar.cpp
    core/keychar.h
    core/key.cpp
    core/key.h
    core/lesson.cpp
    core/lesson.h
    core/profile.cpp
    core/profiledataaccess.cpp
    core/profiledataaccess.h
    core/profile.h
    core/resource.cpp
    core/resourcedataaccess.cpp
    core/resourcedataaccess.h
    core/resource.h
    core/specialkey.cpp
    core/specialkey.h
    core/trainingstats.cpp
    core/trainingstats.h
    core/userdataaccess.cpp
    core/userdataaccess.h
    customlessoneditordialog.cpp
    customlessoneditordialog.h
    declarativeitems/griditem.cpp
    declarativeitems/griditem.h
    declarativeitems/kcolorschemeproxy.cpp
    declarativeitems/kcolorschemeproxy.h
    declarativeitems/lessonpainter.cpp
    declarativeitems/lessonpainter.h
    declarativeitems/lessontexthighlighteritem.cpp
    declarativeitems/lessontexthighlighteritem.h
    declarativeitems/preferencesproxy.cpp
    declarativeitems/preferencesproxy.h
    declarativeitems/scalebackgrounditem.cpp
    declarativeitems/scalebackgrounditem.h
    declarativeitems/traininglinecore.cpp
    declarativeitems/traininglinecore.h
    editor/abstracteditor.cpp
    editor/abstracteditor.h
    editor/charactersviewdelegate.cpp
    editor/charactersviewdelegate.h
    editor/courseeditor.cpp
    editor/courseeditor.h
    editor/keyboardlayoutcombobox.cpp
    editor/keyboardlayoutcombobox.h
    editor/keyboardlayouteditor.cpp
    editor/keyboardlayouteditor.h
    editor/keyboardlayouteditorview.cpp
    editor/keyboardlayouteditorview.h
    editor/keyboardlayoutpropertieswidget.cpp
    editor/keyboardlayoutpropertieswidget.h
    editor/lessontexteditor.cpp
    editor/lessontexteditor.h
    editor/lessontexthighlighter.cpp
    editor/lessontexthighlighter.h
    editor/newcoursewidget.cpp
    editor/newcoursewidget.h
    editor/newkeyboardlayoutwidget.cpp
    editor/newkeyboardlayoutwidget.h
    editor/newresourceassistant.cpp
    editor/newresourceassistant.h
    editor/resourceeditor.cpp
    editor/resourceeditor.h
    editor/resourceeditorwidget.cpp
    editor/resourceeditorwidget.h
    editor/resourcetemplatewidget.cpp
    editor/resourcetemplatewidget.h
    editor/resourcetypeswidget.cpp
    editor/resourcetypeswidget.h
    ktouchcontext.cpp
    ktouchcontext.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    models/categorizedresourcesortfilterproxymodel.cpp
    models/categorizedresourcesortfilterproxymodel.h
    models/charactersmodel.cpp
    models/charactersmodel.h
    models/errorsmodel.cpp
    models/errorsmodel.h
    models/learningprogressmodel.cpp
    models/learningprogressmodel.h
    models/lessonmodel.cpp
    models/lessonmodel.h
    models/resourcemodel.cpp
    models/resourcemodel.h
    trainingconfigwidget.cpp
    trainingconfigwidget.h
    undocommands/coursecommands.cpp
    undocommands/coursecommands.h
    undocommands/keyboardlayoutcommands.cpp
    undocommands/keyboardlayoutcommands.h)

qt_add_resources(ktouch_imgs_SRCS images/images.qrc)

if (Qt5QuickCompiler_FOUND AND COMPILE_QML)
    qtquick_compiler_add_resources(ktouch_qml_SRCS qml/qml.qrc)
else ()
    qt_add_resources(ktouch_qml_SRCS qml/qml.qrc)
endif()

# compile UI files
ki18n_wrap_ui(ktouch_SRCS
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
    ui/customlessoneditordialog.ui
)

if (KTOUCH_BUILD_WITH_X11)
    add_definitions(-DKTOUCH_BUILD_WITH_X11)
    include_directories(${X11_Xkb_INCLUDE_PATH})
    include_directories(${X11_Xkbfile_INCLUDE_PATH})
    set(ktouch_SRCS ${ktouch_SRCS} x11_helper.cpp x11_helper.h)
    set(ktouch_X11_DEPS Qt::X11Extras ${X11_Xkb_LIB} ${X11_LIBRARIES} XCB::XCB XCB::XKB)
endif ()

kconfig_add_kcfg_files(ktouch_SRCS preferences.kcfgc)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/*-apps-ktouch.png")
ecm_add_app_icon(ktouch_SRCS ICONS ${ICONS_SRCS})

add_executable(ktouch ${ktouch_SRCS} ${ktouch_imgs_SRCS} ${ktouch_qml_SRCS})

#uncomment this if oxygen icons for ktouch are available
target_link_libraries(ktouch
    LINK_PUBLIC
        Qt::Qml
        Qt::Quick
        Qt::QuickWidgets
        Qt::QuickControls2
        Qt::Sql
        Qt::XmlPatterns
        KF5::Completion
        KF5::ConfigWidgets
        KF5::ItemViews
        KF5::XmlGui
        KF5::I18n
        KF5::KCMUtils
        KF5::TextWidgets
        KF5::WindowSystem
        KF5::CoreAddons
        KF5::IconThemes
        ${ktouch_X11_DEPS}
)

#kde4_add_app_icon(ktouch_SRCS "${KDE4_ICON_DIR}/oxygen/*/apps/ktouch.png")
#kde4_add_app_icon(ktouch_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-ktouch.png")

install(TARGETS ktouch ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ktouch.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})


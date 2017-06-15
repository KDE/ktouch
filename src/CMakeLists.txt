ecm_setup_version(${KDE_APPLICATIONS_VERSION}
    VARIABLE_PREFIX KTOUCH
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KTouchConfigVersion.cmake"
)

find_package(X11)
add_feature_info("X11-Xkbfile" X11_Xkbfile_FOUND "required for automatic keyboard layout detection")

find_package(XCB OPTIONAL_COMPONENTS XCB XKB)
add_feature_info("XCB-XKB" XCB_XKB_FOUND "required for automatic keyboard layout detection")

ecm_optional_add_subdirectory(schemata)

# set include directories
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${ktouch_SOURCE_DIR}
)


# configure the local configuration file
configure_file(ktouch_build_config.h.in ktouch_build_config.h)


# set the source code files from which KTouch is compiled
set(ktouch_SRCS
    main.cpp
    application.cpp
    mainwindow.cpp
    bindings/utils.cpp
    bindings/stringformatter.cpp
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
    customlessoneditordialog.cpp
    ktouchcontext.cpp
)

qt5_add_resources(ktouch_SRCS qml/qml.qrc)

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

set (KTOUCH_BUILD_WITH_X11 X11_Xkbfile_FOUND AND XCB_XKB_FOUND AND XCB_FOUND)

if (KTOUCH_BUILD_WITH_X11)
    add_definitions(-DKTOUCH_BUILD_WITH_X11)
    include_directories(${X11_Xkbfile_INCLUDE_PATH})
    set(ktouch_SRCS ${ktouch_SRCS} x11_helper.cpp)
else (KTOUCH_BUILD_WITH_X11)
    set(ktouch_SRCS ${ktouch_SRCS} keyboardlayoutmenu.cpp)
endif (KTOUCH_BUILD_WITH_X11)

kconfig_add_kcfg_files(ktouch_SRCS preferences.kcfgc)

add_executable(ktouch ${ktouch_SRCS})

set(ktouch_X11_DEPS "")

if (KTOUCH_BUILD_WITH_X11)
    set(ktouch_X11_DEPS ${X11_Xkbfile_LIB} ${X11_LIBRARIES} XCB::XCB XCB::XKB)
endif (KTOUCH_BUILD_WITH_X11)


#uncomment this if oxygen icons for ktouch are available
target_link_libraries(ktouch
    LINK_PUBLIC
        Qt5::Qml
        Qt5::Quick
        Qt5::QuickWidgets
        Qt5::Script
        Qt5::Sql
        Qt5::XmlPatterns
        Qt5::X11Extras
        KF5::Completion
        KF5::ConfigWidgets
        KF5::Declarative
        KF5::ItemViews
        KF5::XmlGui
        KF5::I18n
        KF5::KCMUtils
        KF5::TextWidgets
        KF5::WindowSystem
        KF5::CoreAddons
        ${ktouch_X11_DEPS}
)

#kde4_add_app_icon(ktouch_SRCS "${KDE4_ICON_DIR}/oxygen/*/apps/ktouch.png")
#kde4_add_app_icon(ktouch_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-ktouch.png")

install(TARGETS ktouch ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ktouch.kcfg DESTINATION ${KCFG_INSTALL_DIR})


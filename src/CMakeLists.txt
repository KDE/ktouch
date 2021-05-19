ecm_setup_version(${RELEASE_SERVICE_VERSION}
    VARIABLE_PREFIX KTOUCH
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KTouchConfigVersion.cmake"
)

find_package(X11)
find_package(XCB OPTIONAL_COMPONENTS XCB XKB)

if (Qt5X11Extras_FOUND AND X11_FOUND AND X11_Xkbfile_FOUND AND X11_Xkb_FOUND AND XCB_FOUND AND XCB_XKB_FOUND)
    set(KTOUCH_BUILD_WITH_X11 TRUE)
endif()

add_feature_info ("Keyboard Layout Detection" KTOUCH_BUILD_WITH_X11 "needs Qt5X11Extras, libX11, libxkb, libxkbfile, libxcb, and libxcb-xkb")

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
    declarativeitems/kcolorschemeproxy.cpp
    declarativeitems/lessonpainter.cpp
    declarativeitems/lessontexthighlighteritem.cpp
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
    set(ktouch_SRCS ${ktouch_SRCS} x11_helper.cpp)
    set(ktouch_X11_DEPS Qt::X11Extras ${X11_Xkb_LIB} ${X11_LIBRARIES} XCB::XCB XCB::XKB)
endif ()

kconfig_add_kcfg_files(ktouch_SRCS preferences.kcfgc)

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


# KTouch CMakeLists.txt file
#
# KDE Application Version, managed by release script
set (RELEASE_SERVICE_VERSION_MAJOR "25")
set (RELEASE_SERVICE_VERSION_MINOR "08")
set (RELEASE_SERVICE_VERSION_MICRO "1")
set (RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")


cmake_minimum_required(VERSION 3.16)
project(ktouch VERSION ${RELEASE_SERVICE_VERSION})

set(QT_MIN_VERSION "6.6.0")
set(KF_MIN_VERSION "6.0.0")
set (KDE_COMPILERSETTINGS_LEVEL "6.0.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
find_package(KF6DocTools)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} )

include(KDEInstallDirs)
include(ECMAddTests)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMAddAppIcon)
include(ECMInstallIcons)
include(ECMOptionalAddSubdirectory)
include(ECMSetupVersion)
include(FeatureSummary)
include(ECMDeprecationSettings)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    QuickWidgets
    QuickControls2
    Sql
    Widgets
    Xml
)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    Completion
    Config
    ConfigWidgets
    CoreAddons
    I18n
    ItemViews
    KCMUtils
    TextWidgets
    WidgetsAddons
    WindowSystem
    XmlGui
)

# Needs https://invent.kde.org/libraries/kqtquickcharts/-/merge_requests/10
# to actually work with 6.0.0.
find_package(KQtQuickCharts)
#find_package(KQtQuickCharts 6.0.0) # aka >= 25.07.80
set_package_properties(KQtQuickCharts PROPERTIES
    DESCRIPTION "QtQuick plugin to render beautiful and interactive charts"
    URL "https://commits.kde.org/kqtquickcharts"
    TYPE RUNTIME
)

find_package(LibXml2 REQUIRED)

option(WITHOUT_X11 "Build without X11 Keyboard Layout Detection (skips finding X11)" OFF)

if (NOT WITHOUT_X11)
    find_package(X11)
    find_package(XCB OPTIONAL_COMPONENTS XCB XKB)
    if (Qt6Gui_VERSION VERSION_GREATER_EQUAL 6.9.0)
        find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS GuiPrivate)
    else()
        set(Qt6GuiPrivate_FOUND TRUE)
    endif()

    if (Qt6GuiPrivate_FOUND AND X11_FOUND AND X11_Xkbfile_FOUND AND X11_Xkb_FOUND AND XCB_FOUND AND XCB_XKB_FOUND)
        set(KTOUCH_BUILD_WITH_X11 TRUE)
    endif()

    add_feature_info ("Keyboard Layout Detection" KTOUCH_BUILD_WITH_X11 "needs Qt6GuiPrivate, libX11, libxkb, libxkbfile, libxcb, and libxcb-xkb")
endif()

# enable QML debugging for debug builds
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DQT_QML_DEBUG")

ecm_set_disabled_deprecation_versions(QT 5.15.2  KF 5.101.0)

# subdirectories to build
ecm_optional_add_subdirectory(data)
ecm_optional_add_subdirectory(src)
# ecm_optional_add_subdirectory(sounds)
ecm_optional_add_subdirectory(icons)

# files to install in the ktouch project root directory
ki18n_install(po)
if (KF6DocTools_FOUND)
    ecm_optional_add_subdirectory(doc)
    kdoctools_install(po)
endif()
install( PROGRAMS org.kde.ktouch.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install(FILES org.kde.ktouch.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

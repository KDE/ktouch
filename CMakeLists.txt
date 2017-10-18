# KTouch CMakeLists.txt file
#
# KDE Application Version, managed by release script
set (KDE_APPLICATIONS_VERSION_MAJOR "17")
set (KDE_APPLICATIONS_VERSION_MINOR "11")
set (KDE_APPLICATIONS_VERSION_MICRO "70")
set (KDE_APPLICATIONS_VERSION "${KDE_APPLICATIONS_VERSION_MAJOR}.${KDE_APPLICATIONS_VERSION_MINOR}.${KDE_APPLICATIONS_VERSION_MICRO}")


cmake_minimum_required(VERSION 3.0)
project(ktouch VERSION ${KDE_APPLICATIONS_VERSION})

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
find_package(KF5DocTools)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(ECMAddTests)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMInstallIcons)
include(ECMOptionalAddSubdirectory)
include(ECMSetupVersion)
include(FeatureSummary)

find_package(Qt5 5.5 REQUIRED COMPONENTS
    Gui
    Qml
    Quick
    QuickWidgets
    Script
    Sql
    Test
    Widgets
    X11Extras
    Xml
    XmlPatterns
)

find_package(KF5 REQUIRED COMPONENTS
    Completion
    Config
    ConfigWidgets
    CoreAddons
    Declarative
    DocTools
    I18n
    ItemViews
    KCMUtils
    TextWidgets
    WidgetsAddons
    WindowSystem
    XmlGui
)

# enable QML debugging for debug builds
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DQT_QML_DEBUG")

# subdirectories to build
ecm_optional_add_subdirectory(data)
ecm_optional_add_subdirectory(doc)
ecm_optional_add_subdirectory(src)
# ecm_optional_add_subdirectory(sounds)
ecm_optional_add_subdirectory(images)
ecm_optional_add_subdirectory(icons)

# files to install in the ktouch project root directory
install( PROGRAMS org.kde.ktouch.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install(FILES org.kde.ktouch.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

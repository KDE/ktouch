# KTouch CMakeLists.txt file
#

project(ktouch)

# KDE Application Version, managed by release script
set (KDE_APPLICATIONS_VERSION_MAJOR "16")
set (KDE_APPLICATIONS_VERSION_MINOR "11")
set (KDE_APPLICATIONS_VERSION_MICRO "90")
set (KDE_APPLICATIONS_VERSION "${KDE_APPLICATIONS_VERSION_MAJOR}.${KDE_APPLICATIONS_VERSION_MINOR}.${KDE_APPLICATIONS_VERSION_MICRO}")


cmake_minimum_required(VERSION 2.8.12)
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
    Widgets
    Sql
    Script
    XmlPatterns
    Qml
    Quick
    QuickWidgets
    Test
    X11Extras
)

find_package(KF5 REQUIRED COMPONENTS
    Config
    ConfigWidgets
    CoreAddons
    Declarative
    DocTools
    I18n
    KIO
    NewStuff
    WidgetsAddons
    XmlGui
    ItemViews
    KCMUtils
    TextEditor
    WindowSystem
)

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

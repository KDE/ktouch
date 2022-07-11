# KTouch CMakeLists.txt file
#
# KDE Application Version, managed by release script
set (RELEASE_SERVICE_VERSION_MAJOR "22")
set (RELEASE_SERVICE_VERSION_MINOR "07")
set (RELEASE_SERVICE_VERSION_MICRO "70")
set (RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")


cmake_minimum_required(VERSION 3.15)
project(ktouch VERSION ${RELEASE_SERVICE_VERSION})

set(QT_MIN_VERSION "5.15.0")
set(KF5_MIN_VERSION "5.90.0")
set (KDE_COMPILERSETTINGS_LEVEL "5.82.0")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
find_package(KF5DocTools)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} )

include(KDEInstallDirs)
include(ECMAddTests)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMInstallIcons)
include(ECMOptionalAddSubdirectory)
include(ECMSetupVersion)
include(FeatureSummary)

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    QuickWidgets
    QuickControls2
    Sql
    Widgets
    Xml
    XmlPatterns
)
if (QT_MAJOR_VERSION STREQUAL "5")
find_package(Qt5X11Extras)
endif()
find_package(Qt${QT_MAJOR_VERSION} CONFIG QUIET OPTIONAL_COMPONENTS QuickCompiler)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Completion
    Config
    ConfigWidgets
    CoreAddons
    DocTools
    I18n
    ItemViews
    KCMUtils
    TextWidgets
    WidgetsAddons
    WindowSystem
    XmlGui
    IconThemes
)


option(COMPILE_QML "Precompile QML code" OFF)

# enable QML debugging for debug builds
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DQT_QML_DEBUG")

# subdirectories to build
ecm_optional_add_subdirectory(data)
ecm_optional_add_subdirectory(doc)
ecm_optional_add_subdirectory(src)
# ecm_optional_add_subdirectory(sounds)
ecm_optional_add_subdirectory(icons)

# files to install in the ktouch project root directory
ki18n_install(po)
if (KF5DocTools_FOUND)
    kdoctools_install(po)
endif()
install( PROGRAMS org.kde.ktouch.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install(FILES org.kde.ktouch.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

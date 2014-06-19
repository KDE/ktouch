# KTouch CMakeLists.txt file
#

project(ktouch)

find_package(KDE4 4.9.95 REQUIRED)
include(KDE4Defaults)

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES} ${QT4_INCLUDES})
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

# subdirectories to build
add_subdirectory(data)
add_subdirectory(doc)
add_subdirectory(src)
# add_subdirectory(sounds) # excluded because of the lack of sound support
add_subdirectory(images)
add_subdirectory(icons)

# files to install in the ktouch project root directory
install( PROGRAMS ktouch.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install(FILES ktouch.appdata.xml DESTINATION ${SHARE_INSTALL_PREFIX}/appdata/)

macro_display_feature_log()

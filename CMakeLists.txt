# KTouch CMakeList.txt file
#

project(ktouch)

find_package(KDE4 REQUIRED)
find_library(X11_Xkbfile_LIB REQUIRED)
include(KDE4Defaults)

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES} ${QT4_INCLUDES})
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

# subdirectories to build
add_subdirectory(data)
add_subdirectory(doc)
add_subdirectory(src) 
add_subdirectory(keyboards) 
add_subdirectory(training) 
add_subdirectory(sounds) 
add_subdirectory(graphics) 
add_subdirectory(images) 
add_subdirectory(icons) 

# files to install in the ktouch project root directory
install( PROGRAMS ktouch.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )

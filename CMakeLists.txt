# KTouch CMakeList.txt file
#

project( ktouch )

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES} ${QT4_INCLUDES})

# subdirectories to build
add_subdirectory( doc )
add_subdirectory( src ) 
add_subdirectory( keyboards ) 
add_subdirectory( training ) 
add_subdirectory( sounds ) 
add_subdirectory( graphics ) 
add_subdirectory( icons ) 

# files to install in the ktouch project root directory
install( PROGRAMS ktouch.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )

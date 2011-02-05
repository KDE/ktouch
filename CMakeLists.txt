# KTouch CMakeList.txt file
#

project( ktouch )

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

# KTouch CMakeList.txt file
#
# To build outside the KDEEDU package, follow the instructions in the 
# build.readme file.
#

project( ktouch )

add_subdirectory( src ) 
add_subdirectory( keyboards ) 
add_subdirectory( training ) 
add_subdirectory( sounds ) 
add_subdirectory( graphics ) 
add_subdirectory( icons ) 

# files to install in the ktouch project dir
install( FILES ktouch.desktop  DESTINATION  ${XDG_APPS_DIR} )

project(ktouch)

add_subdirectory( src ) 
add_subdirectory( keyboards ) 
add_subdirectory( training ) 
add_subdirectory( sounds ) 
add_subdirectory( graphics ) 
add_subdirectory( icons ) 




########### install files ###############

install( FILES ktouch.desktop  DESTINATION  ${XDG_APPS_DIR} )




#original Makefile.am contents follow:

## This file you have to edit. Change the name here
#SUBDIRS = src keyboards training sounds graphics icons
#
## not a GNU package. You can remove this line, if
## have all needed files, that a GNU package needs
#AUTOMAKE_OPTIONS = foreign
#
## place KTouch in the start menu under edutainment
#xdg_apps_DATA = ktouch.desktop
#
#
#
#include ../admin/Doxyfile.am

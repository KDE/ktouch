project(ktouch)

add_subdirectory( src ) 
add_subdirectory( keyboards ) 
add_subdirectory( training ) 
add_subdirectory( sounds ) 
add_subdirectory( graphics ) 
add_subdirectory( icons ) 




########### install files ###############

install( FILES ktouch.desktop  DESTINATION  ${XDG_APPS_DIR} )


# KTouch CMakeList.txt file
#
# To build outside the KDEEDU package, follow the instructions in the 
# build.readme file.

project (ktouch)

# allow to select a Qt4 only build which does not require kdelibs. This will, 
# however, not provide the same feature set as a proper KDE build
option (COMPILE_WITH_QT4_ONLY   "Build only with Qt 4 libraries."  off)

# on Windows, set release mode as default
if (WIN32)

  if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release CACHE STRING
          "Release build is the default for Windows builds."
        )
  endif (NOT CMAKE_BUILD_TYPE)

endif (WIN32)

# add subdirectory for building kdeeduplot library if building a Qt4 only build
# and some additional options
if (COMPILE_WITH_QT4_ONLY)

	# These variables are supposed to set the include paths and link libraries
	# Variables must be set _before_ FIND_PACKAGE( QT4 )
	set( QT_USE_QTMAIN TRUE )

	# Set all variables for QT4 builds
	find_package( Qt4 REQUIRED )
	include( ${QT_USE_FILE} )

	add_subdirectory ( ../libkdeedu/kdeeduplot ${CMAKE_BINARY_DIR}/kdeeduplot )

else (COMPILE_WITH_QT4_ONLY)

	add_subdirectory( src ) 
	add_subdirectory( keyboards ) 
	add_subdirectory( training ) 
	add_subdirectory( sounds ) 
	add_subdirectory( graphics ) 
	add_subdirectory( icons ) 

	# files to install in the ktouch project dir
	install( FILES ktouch.desktop  DESTINATION  ${XDG_APPS_DIR} )

endif (COMPILE_WITH_QT4_ONLY)


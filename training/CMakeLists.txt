kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/ktouch FILES catalan.ktouch.xml  danish2.ktouch.xml  danish.ktouch.xml  dvorak_ABCD.ktouch.xml  dvorak_es.ktouch.xml  dvorak.ktouch.xml  english.ktouch.xml  french2.ktouch.xml  french.ktouch.xml  german2.ktouch.xml  german3.ktouch.xml  german.ktouch.xml  german.neo.ktouch.xml  german.number.ktouch.xml  italian.ktouch.xml  nederlands_junior.ktouch.xml  nederlands.ktouch.xml  norwegian.ktouch.xml  polish.ktouch.xml  spanish.ktouch.xml )

kde4_footer()



#original Makefile.am contents follow:

## adding training files
#trainingdir = $(kde_datadir)/ktouch
#training_DATA = \
#catalan.ktouch.xml \
#danish2.ktouch.xml \
#danish.ktouch.xml \
#dvorak_ABCD.ktouch.xml \
#dvorak_es.ktouch.xml \
#dvorak.ktouch.xml \
#english.ktouch.xml \
#french2.ktouch.xml \
#french.ktouch.xml \
#german2.ktouch.xml \
#german3.ktouch.xml \
#german.ktouch.xml \
#german.neo.ktouch.xml \
#german.number.ktouch.xml \
#italian.ktouch.xml \
#nederlands_junior.ktouch.xml \
#nederlands.ktouch.xml \
#norwegian.ktouch.xml \
#polish.ktouch.xml \
#spanish.ktouch.xml

FILE(GLOB wavfile *.wav)
install( FILES ${wavfile}  DESTINATION  ${KDE_INSTALL_DATADIR}/ktouch )

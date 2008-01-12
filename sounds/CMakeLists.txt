FILE(GLOB wavfile *.wav)
install( FILES ${wavfile}  DESTINATION  ${DATA_INSTALL_DIR}/ktouch )

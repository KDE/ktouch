FILE(GLOB schemata *.xsd)
install( FILES ${schemata} DESTINATION ${DATA_INSTALL_DIR}/ktouch/schemata )

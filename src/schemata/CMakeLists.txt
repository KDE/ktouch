FILE(GLOB schemata *.xsd)
install( FILES ${schemata} DESTINATION ${KDE_INSTALL_DATADIR}/ktouch/schemata )

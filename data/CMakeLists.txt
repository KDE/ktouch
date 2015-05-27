ecm_optional_add_subdirectory(keyboardlayouts)
ecm_optional_add_subdirectory(courses)

install( FILES "data.xml" DESTINATION ${DATA_INSTALL_DIR}/ktouch )

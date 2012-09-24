project(graph)

set(graph_SRCS
    graphplugin.cpp
    )

automoc4_add_library(graphplugin SHARED ${graph_SRCS})
target_link_libraries(graphplugin ${KDE4_PLASMA_LIBS} ${QT_QTSCRIPT_LIBRARY} ${QT_QTDECLARATIVE_LIBRARY} kdeclarative)

install(TARGETS graphplugin DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/ktouch/graph)

set(ktouch_graphplugin_files
    qml/qmldir

    qml/GraphItem.qml
)

install(FILES ${ktouch_graphplugin_files} DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/ktouch/graph)

set (coursefiles
    ara.xml
    bg1.xml
    bg2.xml
    br.xml
    cat.xml
    ch1.xml
    ch2.xml
    cz.xml
    de.dvorak.xml
    de.neo.xml
    de1.xml
    de2.xml
    dk1.xml
    es.dvorak.xml
    es1.xml
    es2.xml
    es3.xml
    es4.xml
    fi.xml
    fr.bepo.xml
    fr.dvarok.xml
    fr.xml
    gb.xml
    gr.xml
    ir.xml
    it.xml
    it2.xml
    lt.std.xml
    nl1.xml
    nl2.xml
    no.xml
    pl.xml
    ru.legacy.xml
    ru1.xml
    ru2.xml
    si.xml
    sk.xml
    tr.xml
    ua.xml
    us.colemak.xml
    us.dvorak1.xml
    us.dvorak2.xml
    us.workman.xml
    us.xml
)

file(GLOB coursefiles_GLOB RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.xml)
list(SORT coursefiles_GLOB)
if(NOT "${coursefiles}" STREQUAL "${coursefiles_GLOB}")
    message(AUTHOR_WARNING "List of .xml files in data/courses differs from the list of courses")
endif()

install( FILES ${coursefiles} DESTINATION ${KDE_INSTALL_DATADIR}/ktouch/courses )


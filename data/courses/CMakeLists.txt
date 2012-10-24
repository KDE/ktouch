FILE(GLOB coursesfiles *.xml)
set (coursefiles
    ara.xml
    bg1.xml
    bg2.xml
    br.xml
    cat.xml
    cz.xml
    de1.xml
    de2.xml
    de3.xml
    de.dvorak.xml
    de.neo.xml
    dk1.xml
    es1.xml
    es2.xml
    es3.xml
    es.dvorak.xml
    fi.xml
    fr.auto1.xml
    fr.auto2.xml
    fr.dvarok.xml
    gb.xml
    gr.xml
    it.xml
    lt.std.xml
    nl1.xml
    nl2.xml
    no.xml
    pl.xml
    ru1.xml
    ru2.xml
    ru.legacy.xml
    si.xml
    sk.xml
    tr.xml
    ua.xml
    us.colemak.xml
    us.dvorak1.xml
    us.dvorak2.xml
    us.xml
)

install( FILES ${coursesfiles} DESTINATION ${DATA_INSTALL_DIR}/ktouch/courses )


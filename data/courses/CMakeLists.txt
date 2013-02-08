set (coursefiles
    ara.xml
    bg1.xml
    bg2.xml
    br.xml
    cat.xml
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
    fi.xml
    fr.bepo.xml
    fr.dvarok.xml
    fr.xml
    gb.xml
    gr.xml
    it.xml
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
    us.xml
)

install( FILES ${coursefiles} DESTINATION ${DATA_INSTALL_DIR}/ktouch/courses )


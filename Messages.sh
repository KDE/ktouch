#! /bin/sh
$EXTRACTRC `find src -name '*.rc'` >> rc.cpp
$EXTRACTRC `find src/ui -name *.ui` >> rc.cpp
$EXTRACTRC `find src -name '*.kcfg'` >> rc.cpp
$XGETTEXT rc.cpp src/*.cpp -o $podir/ktouch.pot


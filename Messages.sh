#! /bin/sh
$EXTRACTRC `find src -name '*.rc'` >> rc.cpp
$EXTRACTRC `find src/ui -name *.ui` >> rc.cpp
$EXTRACTRC `find src -name '*.kcfg'` >> rc.cpp
$EXTRACTRC --tag=Title --context "Lesson Name" `find training -name *.ktouch.xml` >> rc.cpp
$EXTRACTRC --tag=Title --context "Keyboard Layout Name" `find keyboards -name *.keyboard.xml` >> rc.cpp
$XGETTEXT rc.cpp src/*.cpp -o $podir/ktouch.pot

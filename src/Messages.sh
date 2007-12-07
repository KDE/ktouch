#! /bin/sh
$EXTRACTRC `find ui -name \*.ui` >> rc.cpp
$XGETTEXT *.cpp -o $podir/ktouch.pot

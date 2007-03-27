/***************************************************************************
 *   ktouchprefsound.h                                                     *
 *   ---------------------                                                 *
 *   Copyright (C) 2007 by Håvard Frøiland <haavard@solveien.net>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCH_PREF_SOUND_H
#define KTOUCH_PREF_SOUND_H

#include <QWidget>

#include "ui_ktouchprefsoundpage.h"

class KTouchPrefSound : public QWidget {

public:
    explicit KTouchPrefSound(QWidget *parent)
    : QWidget( parent )
    {
        ui.setupUi(this);
    }

    Ui_KTouchPrefSoundPage ui;
};

#endif

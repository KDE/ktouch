/***************************************************************************
 *   ktouchprefgeneral.h                                                   *
 *   ---------------------                                                 *
 *   Copyright (C) 2006 by Håvard Frøiland                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCH_PREF_GENERAL_H
#define KTOUCH_PREF_GENERAL_H

#include <QWidget>

#include "ui_ktouchprefgenerallayout.h"

class KTouchPrefGeneral : public QWidget {

public:
    explicit KTouchPrefGeneral(QWidget *parent)
    : QWidget( parent )
    {
        ui.setupUi(this);
    };

    Ui_KTouchPrefGeneralLayout ui;
};

#endif

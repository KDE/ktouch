/***************************************************************************
                          touchEdit.h  -  description
                             -------------------
    begin                : Tue Nov 27 2001
    copyright            : (C) 2001 by Haavard Froeiland
    email                : haavard@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TOUCHEDIT_H
#define TOUCHEDIT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "touchlecture.h"
#include "touchEditLayout.h"

class TouchEdit : public TouchEditLayout {
   Q_OBJECT

public:
	/** constructor */
	TouchEdit(QWidget* parent, TouchLecture* lecture);

	/** destructor */
	~TouchEdit();
	

private:
	TouchLecture* lecture;

public slots: // Public slots
  /** No descriptions */
  void fetchLevel(int);
  /** Moves level one place down */
  void moveDown();
  /** Moves level one place up */
  void moveUp();
private: // Private methods
  /** No descriptions */
  void swapLevel(int,int);
private: // Private methods
  /** Filles up the level list */
  void fillLevelList();
};

#endif



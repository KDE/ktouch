/***************************************************************************
 *   ktouchcoloreditor.h                                                   *
 *   -------------------                                                   *
 *   Copyright (C) 20064 by Andreas Nicolai                                *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHCOLOREDITOR_H
#define KTOUCHCOLOREDITOR_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ktouchcoloreditor_dlg.h"

#include <qvaluelist.h>

#include "ktouchcolorscheme.h"

/// Implementation of the KTouch Color Scheme Editor.
class KTouchColorEditor : public KTouchColorEditorDlg
{
  Q_OBJECT

public:
	/// Constructor
	KTouchColorEditor(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
	/// Destructor
	~KTouchColorEditor();

	/// Use this function to run the dialog.
	/// @param schemes		List with user defined color schemes.
	/// @param active		Index with active color scheme (negative if predefined scheme is active).
	/// @param selected		Here the index of the currently selected scheme is stored.
	void startEditor(QValueList<KTouchColorScheme>& schemes, int active, int & selected);

public slots:
  virtual void          colorSchemeChanged(QListBoxItem * item);
  virtual void          updateClicked();
  virtual void          removeBtnClicked();
  virtual void          addBtnClicked();

protected:
	virtual void closeEvent( QCloseEvent* ce );

private:
	/// Updates the list box with values from m_schemes.
	void updateListBox();
	/// Updates the button controls with data from 'cs', unless NULL is passed, in which case
	/// the controls are disabled (no-color-scheme-selected state).
	void updateControls(const KTouchColorScheme * cs);

	int								m_currentItem;
	QValueList<KTouchColorScheme>	m_schemes;
	bool							m_saveChanges;
};

#endif // KTOUCHCOLOREDITOR_H


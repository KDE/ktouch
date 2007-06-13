/***************************************************************************
 *   ktouchcoloreditor.h                                                   *
 *   -------------------                                                   *
 *   Copyright (C) 2006 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHCOLOREDITORDIALOG_H
#define KTOUCHCOLOREDITORDIALOG_H

#include "ui_ktouchcoloreditordialog.h"

#include <QList>
#include <QDialog>

class QCloseEvent;

#include "ktouchcolorscheme.h"

/// Implementation of the KTouch Color Scheme Editor.
class KTouchColorEditorDialog : public QDialog, public Ui_KTouchColorEditorDialog {
  Q_OBJECT

public:
	/// Constructor
	KTouchColorEditorDialog(QWidget* parent = 0);
	/// Destructor
	~KTouchColorEditorDialog();

	/// Use this function to run the dialog.
	/// @param schemes		List with user defined color schemes.
	/// @param active		Index with active color scheme (negative if predefined scheme is active).
	/// @param selected		Here the index of the currently selected scheme is stored.
	void startEditor(QList<KTouchColorScheme>& schemes, int active, int & selected);

public slots:
  virtual void          colorSchemeChanged(QListWidgetItem * item);
  virtual void          updateClicked();
  virtual void          removeBtnClicked();
  virtual void          addBtnClicked();

protected:
	virtual void closeEvent( QCloseEvent* ce );

private:
	/// Updates the list widget with values from m_schemes.
	void updateListWidget();
	/// Updates the button controls with data from 'cs', unless NULL is passed, in which case
	/// the controls are disabled (no-color-scheme-selected state).
	void updateControls(const KTouchColorScheme * cs);

	int								m_currentItem;
	QList<KTouchColorScheme>		m_schemes;
	bool							m_saveChanges;
};

#endif // KTOUCHCOLOREDITORDIALOG_H


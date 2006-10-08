/***************************************************************************
 *   ktouchusersetup.h                                                     *
 *   -----------------                                                     *
 *   Copyright (C) 2006 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHUSERSETUP_H
#define KTOUCHUSERSETUP_H

#include "ui_ktouchusersetup_dlg.h"

#include <QDialog>

class QStringList;

/// Implementation of the "Setup Users" dialog.
class KTouchUserSetup : public QDialog, public Ui_KTouchUserSetupDlg {
	Q_OBJECT

public:
	/// Default constructor.
	explicit KTouchUserSetup(QWidget* parent);
	/// Default destructor.
	~KTouchUserSetup();

	/// Use this function to show the dialog.
	/// @param user_names A string list with user names. When the dialog is closed
	///                   the stringlist will hold the new user names.
	void showDialog(QStringList & user_names);

public slots:
	/// Called when the user selects a name in the list box (enables/disabled buttons)
	virtual void          userSelected(int);
	///	Called when the user clicks on the add button.
	virtual void          addBtnClicked();
	///	Called when the user clicks on the remove button.
	virtual void          removeBtnClicked();

protected:
	/// Called when the users pressed close, confirms the changes.
	virtual void closeEvent( QCloseEvent* ce );

private:
	QStringList		*m_userNames;	///< Contains the list with user names.
};

#endif


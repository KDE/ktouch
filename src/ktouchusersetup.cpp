/***************************************************************************
 *   ktouchusersetup.cpp                                                   *
 *   -------------------                                                   *
 *   Copyright (C) 2006 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchusersetup.h"
#include "ktouchusersetup.moc"

#include <kinputdialog.h>
#include <klocale.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kpushbutton.h>

#include <QCloseEvent>

KTouchUserSetup::KTouchUserSetup(QWidget* parent)
: QDialog(parent)
{
}
// ----------------------------------------------------------------------------

KTouchUserSetup::~KTouchUserSetup() {
}
// ----------------------------------------------------------------------------

void KTouchUserSetup::showDialog(QStringList & user_names) {
	m_userNames = &user_names;
	userListBox->clear();
	userListBox->insertStringList(user_names);
	exec();
}
// ----------------------------------------------------------------------------


void KTouchUserSetup::userSelected(int index) {
	if (index == -1 || userListBox->count()<=1 ||
		userListBox->selectedItem()==0 ||
		userListBox->selectedItem()->text()==i18n("default user") )	
	{
		removeBtn->setEnabled(false);
	}
	else {
		removeBtn->setEnabled(true);
	}
}
// ----------------------------------------------------------------------------

void KTouchUserSetup::addBtnClicked() {
	bool ok_pressed;
	QString new_name = KInputDialog::getText(i18n("New user..."), 
						i18n("Please enter a unique user name!"),
						QString::null,
						&ok_pressed, /* bool *   ok = 0, */
						this, /*QWidget *   parent = 0, */
						0, /*const char *   name = 0, */
						0, /*QValidator *   validator = 0, */
						QString::null /*const QString &   mask = QString::null*/
					);
	if (ok_pressed) {
		Q3ListBoxItem * lbi = userListBox->findItem(new_name, Q3ListBox::ExactMatch);
		if (lbi) {
			KMessageBox::sorry(this, i18n("This is not a valid (unique) user name!"));
		}
		else {
			userListBox->insertItem(new_name);
			userListBox->setCurrentItem(userListBox->count()-1);
		}
	}
}
// ----------------------------------------------------------------------------

void KTouchUserSetup::removeBtnClicked() {
	Q3ListBoxItem * lbi = userListBox->selectedItem();
	if (!lbi) return;
	int index = userListBox->index(lbi);
	userListBox->removeItem(index);
}
// ----------------------------------------------------------------------------

// Called when the users pressed close, confirms the changes.
void KTouchUserSetup::closeEvent( QCloseEvent* ce ) {
	int result = KMessageBox::questionYesNoCancel(this,
		i18n("Save user list?"));
	switch (result) {
	  case KMessageBox::Cancel : ce->ignore(); return;

	  case KMessageBox::Yes : {
		m_userNames->clear();
		Q3ListBoxItem * i = userListBox->firstItem();
		while (i) {
			m_userNames->append(i->text());
			i = i->next();
		}
	  }
	  break;

	  default : break;
	}
	ce->accept();
}
// ----------------------------------------------------------------------------




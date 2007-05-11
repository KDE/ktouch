/***************************************************************************
 *   ktouchusersetupdialog.cpp                                             *
 *   -------------------------                                             *
 *   Copyright (C) 2006 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchusersetupdialog.h"
#include "ktouchusersetupdialog.moc"

#include <kinputdialog.h>
#include <klocale.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kpushbutton.h>

#include <QCloseEvent>

KTouchUserSetupDialog::KTouchUserSetupDialog(QWidget* parent)
: QDialog(parent)
{
	setupUi(this);
	connect(addBtn, SIGNAL(clicked()), this, SLOT(addBtnClicked()));
	connect(removeBtn, SIGNAL(clicked()), this, SLOT(removeBtnClicked()));
	connect(closeBtn, SIGNAL(clicked()), this, SLOT(close()));
	connect(userListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(userSelected(int)));
}
// ----------------------------------------------------------------------------

KTouchUserSetupDialog::~KTouchUserSetupDialog() {
}
// ----------------------------------------------------------------------------

void KTouchUserSetupDialog::showDialog(QStringList & user_names) {
	m_userNames = &user_names;
	userListWidget->clear();
	userListWidget->insertItems(0, user_names);
	exec();
}
// ----------------------------------------------------------------------------

void KTouchUserSetupDialog::userSelected(int index) {
	if (index == -1 || userListWidget->count()<=1)
	{
		removeBtn->setEnabled(false);
	}
	else {
		removeBtn->setEnabled(true);
	}
}
// ----------------------------------------------------------------------------

void KTouchUserSetupDialog::addBtnClicked() {
	bool ok_pressed;
	QString new_name = KInputDialog::getText(i18n("New User"), 
						i18n("Please enter a unique user name:"),
						QString(),
						&ok_pressed, /* bool *   ok = 0, */
						this, /*QWidget *   parent = 0, */
						0, /*const char *   name = 0, */
						0, /*QValidator *   validator = 0, */
						QString::null /*const QString &   mask = QString::null*/
					);
	if (ok_pressed) {
		QList<QListWidgetItem *> itemlist = userListWidget->findItems(new_name, Qt::MatchExactly);
		if (!itemlist.isEmpty()) {
			KMessageBox::sorry(this, i18n("This is not a valid (unique) user name!"));
		}
		else {
			userListWidget->addItem(new_name);
			userListWidget->setCurrentRow(userListWidget->count()-1);
		}
	}
}
// ----------------------------------------------------------------------------

void KTouchUserSetupDialog::removeBtnClicked() {
	int index = userListWidget->currentRow();
	if (index == -1) return;
	userListWidget->takeItem(index);
}
// ----------------------------------------------------------------------------

// Called when the users pressed close, confirms the changes.
void KTouchUserSetupDialog::closeEvent( QCloseEvent* ce ) {
	int result = KMessageBox::questionYesNoCancel(this,
		i18n("Save user list?"));
	switch (result) {
	  case KMessageBox::Cancel : ce->ignore(); return;

	  case KMessageBox::Yes : {
		m_userNames->clear();
		for (int i=0; i<userListWidget->count(); ++i) {
			m_userNames->append(userListWidget->item(i)->text() );
		}
	  }
	  break;

	  default : break;
	}
	ce->accept();
}
// ----------------------------------------------------------------------------




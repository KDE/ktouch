/***************************************************************************
 *   ktouchkeyboardeditor.cpp                                              *
 *   ------------------------                                              *
 *   Copyright (C) 2000 by Håvard Frøiland, 2004 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net, ghorwin@users.sourceforge.net          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchkeyboardeditor.h"
#include "ktouchkeyboardeditor.moc"

#include <qlabel.h>
#include <qfont.h>

#include <kmessagebox.h>
#include <kfiledialog.h>
#include <klocale.h>
#include <ksqueezedtextlabel.h>
#include <klineedit.h>
#include <ktextedit.h>
#include <kfontdialog.h>
#include <kdebug.h>

#include <utility>

#include "ktouch.h"
#include "ktouchopenrequest.h"

// **************************
// ***** Public functions ***
// **************************

KTouchKeyboardEditor::KTouchKeyboardEditor(QWidget* parent, const char* name, bool modal, WFlags fl)
  : KTouchKeyboardEditorDlg(parent,name, modal,fl)
{
}
// -----------------------------------------------------------------------------

bool KTouchKeyboardEditor::startEditor(const KURL& url) {
    // call open request dialog and load a keyboard and start the dialogs event loop if
    // the user did not cancel the open request dialog 
    if (openKeyboardFile(url)==QDialog::Accepted)  {
        exec();
        return true;
    }
    else  return false;
}
// -----------------------------------------------------------------------------


// ************************
// ***** Public slots *****
// ************************


void KTouchKeyboardEditor::fontBtnClicked() {
	//kdDebug() << "Fontbutton clicked" << endl;
    QFont f;
    if (KFontDialog::getFont(f, false, this, true)==QDialog::Accepted) {
		m_keyboard.m_fontSuggestions = f.toString();
		// update font and keyboard display
		titleEdit->setFont(f);  
		keyboardCommentEdit->setFont(f);  
		languageEdit->setFont(f);  
		update();	// trigger repaint of the keyboard.
		setModified();
    }
}
// -----------------------------------------------------------------------------

void KTouchKeyboardEditor::openBtnClicked() {
    saveModified();  // save if modified
    openKeyboardFile("");
}
// -----------------------------------------------------------------------------

void KTouchKeyboardEditor::saveBtnClicked() {
    if (m_currentURL.isEmpty()) saveAsBtnClicked();
    else {
        transfer_from_dialog();
        //m_keyboard.saveXML(this, m_currentURL);
        setModified(false);
    }
}
// -----------------------------------------------------------------------------

void KTouchKeyboardEditor::saveAsBtnClicked() {
    QString tmp = KFileDialog::getSaveFileName(QString::null, 
        i18n("*.keyboard.xml|KTouch Keyboard Files (*.keyboard.xml)\n*.*|All Files"), this, i18n("Save Keyboard Layout") );
    if (!tmp.isEmpty()) {
        transfer_from_dialog();
        m_currentURL = tmp;
        //m_keyboard.saveXML(this, m_currentURL);
        setModified(false);
    }
}
// -----------------------------------------------------------------------------

/// Called when the "Add..." button was clicked
void KTouchKeyboardEditor::addBtnClicked() {
}
// -----------------------------------------------------------------------------

/// Called when the "Edit..." button was clicked
void KTouchKeyboardEditor::editBtnClicked() {
}
// -----------------------------------------------------------------------------

/// Called when the "Remove" button was clicked
void KTouchKeyboardEditor::removeBtnClicked() {
}
// -----------------------------------------------------------------------------

/// Called when the selection in the key list box has changed
void KTouchKeyboardEditor::keySelectionChanged(QListBoxItem * item) {
}
// -----------------------------------------------------------------------------



void KTouchKeyboardEditor::paintEvent(QPaintEvent *) {
	
}
// -----------------------------------------------------------------------------

// ****************************
// ***** Private functions ****
// ****************************

void KTouchKeyboardEditor::transfer_to_dialog() {
	if (m_currentURL.isEmpty()) {
		titleEdit->setText( i18n("untitled keyboard layout") );
		keyboardCommentEdit->setText("");
	}
	else {
		titleEdit->setText(m_keyboard.m_title);
		keyboardCommentEdit->setText(m_keyboard.m_comment);
	}
	languageEdit->setText(m_keyboard.m_language);
	kdDebug() << "Setting font '"<< m_keyboard.m_fontSuggestions <<"'" << endl;
	if (!m_keyboard.m_fontSuggestions.isEmpty()) {
		QFont f;
		f.fromString(m_keyboard.m_fontSuggestions);
		titleEdit->setFont(f);  
		keyboardCommentEdit->setFont(f);
		languageEdit->setFont(f);
	}
	kdDebug() << "Adding key definitions to key list" << endl;
	keyListBox->clear();
	QValueVector<KTouchKey>::iterator it;
	unsigned int min_x = 100000;
	unsigned int max_x = 0;
	unsigned int min_y = 100000;
	unsigned int max_y = 0;
	for( it = m_keyboard.m_keys.begin(); it != m_keyboard.m_keys.end(); ++it ) {
		switch (it->m_type) {
			case KTouchKey::NORMAL : keyListBox->insertItem("N  '" + QString(it->m_primaryChar) + "'"); break;
			case KTouchKey::FINGER : keyListBox->insertItem("F  '" + QString(it->m_primaryChar) + "'"); break;
			default                : keyListBox->insertItem("O  '" + it->m_otherKeyText + "'"); break;
		}
		min_x = std::min<unsigned int>(min_x, it->m_x);
		max_x = std::max<unsigned int>(max_x, it->m_x+it->m_w);
		min_y = std::min<unsigned int>(min_y, it->m_y);
		max_y = std::max<unsigned int>(max_y, it->m_y+it->m_h);
	}
	dimensionsLabel->setText( i18n("Keyboard dimensions: %1 x %2").arg(max_x - min_x).arg(max_y - min_y) );
}
// -----------------------------------------------------------------------------
    
void KTouchKeyboardEditor::transfer_from_dialog() {
	m_keyboard.m_title = titleEdit->text();
	if (m_keyboard.m_title.isEmpty())  m_keyboard.m_title = i18n("untitled keyboard layout");
	m_keyboard.m_comment = keyboardCommentEdit->text();
	m_keyboard.m_language = languageEdit->text();
}
// -----------------------------------------------------------------------------

int KTouchKeyboardEditor::openKeyboardFile(const KURL& url) {
    // First setup the open request dialog
    KTouchOpenRequest dlg(this);
    // Call the dialog
    KURL new_url;
    int result = dlg.requestFileToOpen(new_url,
        i18n("Open keyboard file ..."),
        i18n("Which keyboard file would you like to edit?"),
        i18n("Edit current keyboard:"),
        i18n("Open a default keyboard:"),
        i18n("Open a keyboard file:"),
        i18n("Create new keyboard!"),
        url, KTouchPtr->lectureFiles(), i18n("<no keyboard files available>"));

    if (result == QDialog::Accepted) {
        // Ok, user confirmed the dialog, now lets get the url
        m_currentURL = new_url;
        // Try to load the keyboard, if that fails, we create a new keyboard instead
        if (!m_currentURL.isEmpty() && 
		    !m_keyboard.load(this, m_currentURL) && !m_keyboard.loadXML(this, m_currentURL)) 
		{
            KMessageBox::sorry(this, i18n("Could not open the keyboard file, creating a new one instead!"));
            m_currentURL = QString::null; // new keyboards haven't got a URL
			m_keyboard.clear();
        }
        // If we have no URL, we create a new keyboard - can happen if either the user
        // chose "new keyboard" or the chosen keyboard could not be opened
        if (m_currentURL.isEmpty())  {
            m_keyboard.clear();
            setModified(true); // new keyboards are modified by default
        }
        else
            setModified(false); // newly read keyboards are not modified in the begin
        transfer_to_dialog();    // Update our editor with the keyboard data
        return QDialog::Accepted;
    }
    else return QDialog::Rejected;
}
// -----------------------------------------------------------------------------

void KTouchKeyboardEditor::setModified(bool flag) {
    m_modified = flag;
    if (!m_currentURL.isEmpty()) {
        if (flag) this->setCaption("KTouch Keyboard Editor - " + m_currentURL.url() + i18n(" (modified)"));
        else      this->setCaption("KTouch Keyboard Editor - " + m_currentURL.url());
    }
	else
		this->setCaption("KTouch Keyboard Editor - " + i18n("<unnamed keyboard file>"));
}
// -----------------------------------------------------------------------------

bool KTouchKeyboardEditor::saveModified() {
    if (!m_modified) return true;
    // ok, ask the user to save the changes
    int result = KMessageBox::questionYesNoCancel(this, 
        i18n("The keyboard has been changed. Do you want to save the changes?"),QString::null,KStdGuiItem::save(),KStdGuiItem::discard());
    if (result == KMessageBox::Cancel) return false; // User aborted
    if (result == KMessageBox::Yes) saveBtnClicked();
    // if successfully saved the modified flag will be resetted in the saveBtnClicked() function
    return true; // User acknowledged
}
// -----------------------------------------------------------------------------

/***************************************************************************
 *   ktouchkeyboardeditordialog.cpp                                        *
 *   ------------------------------                                        *
 *   Copyright (C) 2000-2007 by Håvard Frøiland and Andreas Nicolai        *
 *   haavard@users.sourceforge.net, ghorwin@users.sourceforge.net          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchkeyboardeditordialog.h"
#include "ktouchkeyboardeditordialog.moc"

#include <QLabel>
#include <QFont>
#include <QTimer>

#include <kmessagebox.h>
#include <kfiledialog.h>
#include <klocale.h>
#include <kfontdialog.h>
#include <kdebug.h>

#include "ktouch.h"
#include "ktouchopenrequestdialog.h"
#include "ktouchkey.h"
#include "ktouchkeyconnector.h"

// **************************
// ***** Public functions ***
// **************************

KTouchKeyboardEditorDialog::KTouchKeyboardEditorDialog(QWidget* parent, Qt::WFlags fl)
  : QDialog(parent,fl)
{
	setupUi(this);

    m_scene = new QGraphicsScene(this);
	keyboardView->setScene(m_scene);
    keyboardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    keyboardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    keyboardView->setRenderHint(QPainter::TextAntialiasing);
    keyboardView->setBackgroundBrush(palette().brush(QPalette::Window));
    keyboardView->setFrameStyle(QFrame::NoFrame);

	// setup the key edit fields
	keyTypeCombo->clear();
	for (int i=0; i<=KTouchKey::Other; ++i) {
		keyTypeCombo->addItem(KTouchKey::keyTypeString(static_cast<KTouchKey::keytype_t>(i)));
	}

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()) );
	// TODO : setup context menu
}
// -----------------------------------------------------------------------------

bool KTouchKeyboardEditorDialog::startEditor(const KUrl& url) {
    // call open request dialog and load a keyboard and start the dialogs event loop if
    // the user did not cancel the open request dialog 
    if (openKeyboardFile(url)==QDialog::Accepted)  {
		m_currentEditKey = NULL;
        exec();
		// Even if the user cancels the dialog we must assume that a keyboard layout 
		// was changed and save to disk. Thus, to be save, we return 'true' and
		// indicate that the keyboard has to be updated.
        return true; 
    }
    else  return false;
}
// -----------------------------------------------------------------------------


// ************************
// ***** Public slots *****
// ************************


void KTouchKeyboardEditorDialog::on_setFontButton_clicked() {
	//kDebug() << "Fontbutton clicked" << endl;
    QFont f;
    if (KFontDialog::getFont(f)==QDialog::Accepted) {
		m_keyboard.m_fontSuggestions = f.toString();
		m_keyboard.setFont(f);
		// update font
		titleEdit->setFont(f);  
		commentEdit->setFont(f);  
		langIDEdit->setFont(f);  
		update();	// trigger repaint of the keyboard.
		setModified();
    }
}
// -----------------------------------------------------------------------------

void KTouchKeyboardEditorDialog::on_openButton_clicked() {
    saveModified();  // save if modified
    openKeyboardFile(KUrl(""));
}
// -----------------------------------------------------------------------------

void KTouchKeyboardEditorDialog::on_saveButton_clicked() {
    if (m_currentURL.isEmpty()) on_saveAsButton_clicked();
    else {
        transfer_from_dialog();
        m_keyboard.saveXML(this, m_currentURL);
        setModified(false);
    }
}
// -----------------------------------------------------------------------------

void KTouchKeyboardEditorDialog::on_saveAsButton_clicked() {
    QString tmp = KFileDialog::getSaveFileName(m_currentURL, 
        i18n("*.keyboard.xml|KTouch Keyboard Files (*.keyboard.xml)\n*.*|All Files"), this, i18n("Save Keyboard Layout") );
    if (!tmp.isEmpty()) {
        transfer_from_dialog();
        m_currentURL = tmp;
        m_keyboard.saveXML(this, m_currentURL);
        setModified(false);
    }
}
// -----------------------------------------------------------------------------

void KTouchKeyboardEditorDialog::on_topLeftChar_textEdited(const QString & text) {
	if (m_keyboard.m_keys.contains(m_currentEditKey)) {
		if (!text.isEmpty())
			m_currentEditKey->m_keyChar[KTouchKey::TopLeft] = text[0];
		else
			m_currentEditKey->m_keyChar[KTouchKey::TopLeft] = QChar();
		m_currentEditKey->update();
	}
}
// -----------------------------------------------------------------------------

void KTouchKeyboardEditorDialog::on_topRightChar_textEdited(const QString & text) {
	if (m_keyboard.m_keys.contains(m_currentEditKey)) {
		if (!text.isEmpty())
			m_currentEditKey->m_keyChar[KTouchKey::TopRight] = text[0];
		else
			m_currentEditKey->m_keyChar[KTouchKey::TopRight] = QChar();
		m_currentEditKey->update();
	}
}
// -----------------------------------------------------------------------------

void KTouchKeyboardEditorDialog::on_bottomLeftChar_textEdited(const QString & text) {
	if (m_keyboard.m_keys.contains(m_currentEditKey)) {
		if (!text.isEmpty())
			m_currentEditKey->m_keyChar[KTouchKey::BottomLeft] = text[0];
		else
			m_currentEditKey->m_keyChar[KTouchKey::BottomLeft] = QChar();
		m_currentEditKey->update();
	}
}
// -----------------------------------------------------------------------------

void KTouchKeyboardEditorDialog::on_bottomRightChar_textEdited(const QString & text) {
	if (m_keyboard.m_keys.contains(m_currentEditKey)) {
		if (!text.isEmpty())
			m_currentEditKey->m_keyChar[KTouchKey::BottomRight] = text[0];
		else
			m_currentEditKey->m_keyChar[KTouchKey::BottomRight] = QChar();
		m_currentEditKey->update();
	}
}
// -----------------------------------------------------------------------------

void KTouchKeyboardEditorDialog::on_keyTypeCombo_currentIndexChanged(int index) {
	if (index == KTouchKey::Other) {
		keyTextLabel->setEnabled(true);
		keyTextEdit->setEnabled(true);
	}
	else {
		keyTextLabel->setEnabled(false);
		keyTextEdit->setEnabled(false);
	}
	if (m_keyboard.m_keys.contains(m_currentEditKey)) {
		m_currentEditKey->m_type = static_cast<KTouchKey::keytype_t>(index);
		m_currentEditKey->update();
	}
}
// -----------------------------------------------------------------------------

void KTouchKeyboardEditorDialog::on_keyTextEdit_textEdited(const QString & text) {
	if (m_keyboard.m_keys.contains(m_currentEditKey)) {
		m_currentEditKey->m_keyText = text;
		m_currentEditKey->update();
	}
}
// -----------------------------------------------------------------------------

void KTouchKeyboardEditorDialog::resizeKeyboard() {
    QRectF sbr = m_scene->itemsBoundingRect();
    qreal scale = qMin(keyboardView->width()/sbr.width(), keyboardView->height()/sbr.height()) * 0.9;

    QMatrix matrix;
    matrix.scale(scale, scale);
    keyboardView->setMatrix(matrix);
}
// -----------------------------------------------------------------------------

void KTouchKeyboardEditorDialog::keyClicked(KTouchKey * k) {
	// update the character edits
	if (m_keyboard.m_keys.contains(m_currentEditKey)) {
		m_currentEditKey->m_state = KTouchKey::NormalState;
		m_currentEditKey->update();
	}
	m_currentEditKey = k;
	if (m_keyboard.m_keys.contains(m_currentEditKey)) {
		m_currentEditKey->m_state = KTouchKey::HighlightedState;
		m_currentEditKey->update();
		
		// set correct combo type
		if (m_currentEditKey->m_type > KTouchKey::Other) 
			keyTypeCombo->setCurrentIndex( KTouchKey::Other );
		else 
			keyTypeCombo->setCurrentIndex( m_currentEditKey->m_type );
		if (m_currentEditKey->m_type == KTouchKey::Other)
			keyTextEdit->setText( m_currentEditKey->m_keyText );
		on_keyTypeCombo_currentIndexChanged( keyTypeCombo->currentIndex() );
		// fill in the keys in the edit lines
		if (m_currentEditKey->m_keyChar[KTouchKey::TopLeft] != QChar())
			topLeftChar->setText( m_currentEditKey->m_keyChar[KTouchKey::TopLeft] );
		else
			topLeftChar->clear();
		if (m_currentEditKey->m_keyChar[KTouchKey::TopRight] != QChar())
			topRightChar->setText( m_currentEditKey->m_keyChar[KTouchKey::TopRight] );
		else
			topRightChar->clear();
		if (m_currentEditKey->m_keyChar[KTouchKey::BottomLeft] != QChar())
			bottomLeftChar->setText( m_currentEditKey->m_keyChar[KTouchKey::BottomLeft] );
		else
			bottomLeftChar->clear();
		if (m_currentEditKey->m_keyChar[KTouchKey::BottomRight] != QChar())
			bottomRightChar->setText( m_currentEditKey->m_keyChar[KTouchKey::BottomRight] );
		else
			bottomRightChar->clear();
	}
}
// -----------------------------------------------------------------------------

// ********************************
// ***** Event implementations ****
// ********************************

void KTouchKeyboardEditorDialog::resizeEvent(QResizeEvent *) {
	resizeKeyboard();
}
// -----------------------------------------------------------------------------

// ****************************
// ***** Private functions ****
// ****************************

void KTouchKeyboardEditorDialog::transfer_to_dialog() {
	if (m_currentURL.isEmpty()) {
		titleEdit->setText( i18n("untitled keyboard layout") );
		commentEdit->setText("");
	}
	else {
		// check if we have an xml extension
		QString fname = m_currentURL.fileName();
		if (!fname.endsWith(".xml")) {
			int pos = fname.indexOf('.');
			m_keyboard.m_language = fname.left(pos);
			m_keyboard.m_title = fname;
		}
		titleEdit->setText(m_keyboard.m_title);
		commentEdit->setText(m_keyboard.m_comment);
	}
	langIDEdit->setText(m_keyboard.m_language);
	if (!m_keyboard.m_fontSuggestions.isEmpty()) {
		QFont f;
		f.fromString(m_keyboard.m_fontSuggestions);
		titleEdit->setFont(f);
		commentEdit->setFont(f);
		langIDEdit->setFont(f);
	}
	unsigned int min_x = 100000;
	unsigned int max_x = 0;
	unsigned int min_y = 100000;
	unsigned int max_y = 0;
	QList<KTouchKey*>::iterator it;
	for( it = m_keyboard.m_keys.begin(); it != m_keyboard.m_keys.end(); ++it ) {
		KTouchKey * key = *it;
        m_scene->addItem(key);
		connect(key, SIGNAL(clicked(KTouchKey *)), this, SLOT(keyClicked(KTouchKey *)));

		min_x = qMin<unsigned int>(min_x, (*it)->m_x);
		max_x = qMax<unsigned int>(max_x, (*it)->m_x+(*it)->m_w);
		min_y = qMin<unsigned int>(min_y, (*it)->m_y);
		max_y = qMax<unsigned int>(max_y, (*it)->m_y+(*it)->m_h);
	}
	QTimer::singleShot(10, this, SLOT(resizeKeyboard()));
}
// -----------------------------------------------------------------------------
    
void KTouchKeyboardEditorDialog::transfer_from_dialog() {
	m_keyboard.m_title = titleEdit->text();
	if (m_keyboard.m_title.isEmpty())  m_keyboard.m_title = i18n("untitled keyboard layout");
	m_keyboard.m_comment = commentEdit->toPlainText();
	m_keyboard.m_language = langIDEdit->text();
}
// -----------------------------------------------------------------------------

int KTouchKeyboardEditorDialog::openKeyboardFile(const KUrl& url) {
    // First setup the open request dialog
    KTouchOpenRequestDialog dlg(this);
    // Call the dialog
    KUrl new_url;
    int result = dlg.requestFileToOpen(new_url,
        i18n("Open keyboard file ..."),
        i18n("Which keyboard file would you like to edit?"),
        i18n("Edit current keyboard:"),
        i18n("Open a default keyboard:"),
        i18n("Open a keyboard file:"),
        i18n("Create new keyboard"),
        url, KTouchPtr->keyboardFiles(), i18n("<no keyboard files available>"));

    if (result == QDialog::Accepted) {
        // Ok, user confirmed the dialog, now lets get the url
        m_currentURL = new_url;
        // Try to load the keyboard, if that fails, we create a new keyboard instead
        if (!m_currentURL.isEmpty() && 
		    !m_keyboard.load(this, m_currentURL) && !m_keyboard.loadXML(this, m_currentURL)) 
		{
            KMessageBox::sorry(this, i18n("Could not open the keyboard file, creating a new one instead."));
            m_currentURL = ""; // new keyboards haven't got a URL
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

void KTouchKeyboardEditorDialog::setModified(bool flag) {
    m_modified = flag;
    if (!m_currentURL.isEmpty()) {
        if (flag) this->setWindowTitle("KTouch Keyboard Editor - " + m_currentURL.url() + i18n(" (modified)"));
        else      this->setWindowTitle("KTouch Keyboard Editor - " + m_currentURL.url());
    }
	else
		this->setWindowTitle("KTouch Keyboard Editor - " + i18n("<unnamed keyboard file>"));
}
// -----------------------------------------------------------------------------

bool KTouchKeyboardEditorDialog::saveModified() {
    if (!m_modified) return true;
    // ok, ask the user to save the changes
    int result = KMessageBox::questionYesNoCancel(this, 
        i18n("The keyboard has been changed. Do you want to save the changes?"),QString(),KStandardGuiItem::save(),KStandardGuiItem::discard());
    if (result == KMessageBox::Cancel) 
		return false; // User aborted
    if (result == KMessageBox::Yes) 
		on_saveButton_clicked();
    // if successfully saved the modified flag will be reset in the saveBtnClicked() function
    return true; // User acknowledged
}
// -----------------------------------------------------------------------------

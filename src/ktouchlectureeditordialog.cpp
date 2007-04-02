/***************************************************************************
 *   ktouchlectureeditor.cpp                                               *
 *   -----------------------                                               *
 *   Copyright (C) 2004 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchlectureeditordialog.h"
#include "ktouchlectureeditordialog.moc"

#include <QLabel>
#include <QString>
#include <QStringList>

#include <kpushbutton.h>
#include <klineedit.h>
#include <ktextedit.h>
#include <klocale.h>
#include <kdebug.h>
#include <ksqueezedtextlabel.h>
#include <kmessagebox.h>
#include <kcombobox.h>
#include <kfontdialog.h>
#include <kfiledialog.h>

#include <algorithm>  // for std::swap

#include "ktouchlecture.h"
#include "ktouchopenrequestdialog.h"
#include "ktouch.h"


// **************************
// ***** Public functions ***
// **************************

KTouchLectureEditorDialog::KTouchLectureEditorDialog(QWidget *parent)
 : QDialog(parent)
{
    setupUi(this);

    upBtn->setIcon( KIcon("go-up") );
    downBtn->setIcon( KIcon("go-down") );
    deleteBtn->setIcon( KIcon("list-remove") );
    newBtn->setIcon( KIcon("list-add") );

    fontButton->setIcon( KIcon("font") );
    openButton->setIcon( KIcon("document-open") );
    quitButton->setIcon( KIcon("application-exit") );
    saveButton->setIcon( KIcon("document-save") );
    saveAsButton->setIcon( KIcon("document-save-as") );

    levelListWidget->setSortingEnabled(false); // don't sort my level list view!

    connect(levelListWidget, SIGNAL(currentRowChanged(int)),this, SLOT(newSelection(int)) );
    connect(newCharsEdit, SIGNAL(textChanged(const QString&)), this, SLOT(newCharsChanged(const QString&)) );
    connect(newBtn, SIGNAL(clicked()), this, SLOT(newLevel()) );
    connect(deleteBtn, SIGNAL(clicked()), this, SLOT(deleteLevel()) );
    connect(upBtn, SIGNAL(clicked()), this, SLOT(moveUp()) );
    connect(downBtn, SIGNAL(clicked()), this, SLOT(moveDown()) );

    // make the connections for making the lecture modified
    connect(titleEdit, SIGNAL(textChanged(const QString&)), this, SLOT(setModified()) );
    connect(lectureCommentEdit, SIGNAL(textChanged()), this, SLOT(setModified()) );
    connect(levelCommentEdit, SIGNAL(textChanged(const QString&)), this, SLOT(setModified()) );
    connect(linesEdit, SIGNAL(textChanged()), this, SLOT(setModified()) );

    connect(fontButton, SIGNAL(clicked()), this, SLOT(fontBtnClicked()) );
    connect(openButton, SIGNAL(clicked()), this, SLOT(openBtnClicked()) );
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()) );
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveBtnClicked()) );
    connect(saveAsButton, SIGNAL(clicked()), this, SLOT(saveAsBtnClicked()) );
}
// -----------------------------------------------------------------------------

bool KTouchLectureEditorDialog::startEditor(const KUrl& url) {
    // call open request dialog and load a lecture, and start the dialogs event loop if
    // the user did not cancel the open request dialog 
    if (openLectureFile(url)==QDialog::Accepted)  {
        exec();
        return true;
    }
    else  return false;
}
// -----------------------------------------------------------------------------


// *************************
// ***** Private slots *****
// *************************

void KTouchLectureEditorDialog::fontBtnClicked() {
    QFont f(m_lecture.m_fontSuggestions);
    if (KFontDialog::getFont(f, KFontChooser::DisplayFrame, this)==QDialog::Accepted) {
        linesEdit->setFont(f);
        lectureCommentEdit->setFont(f);
        levelCommentEdit->setFont(f);
        newCharsEdit->setFont(f);
        titleEdit->setFont(f);
        levelListWidget->setFont(f);
        m_lecture.m_fontSuggestions = f.family();
        setModified();
    }
}
// -----------------------------------------------------------------------------

void KTouchLectureEditorDialog::openBtnClicked() {
    saveModified();  // save if modified
    openLectureFile(KUrl(""));
}
// -----------------------------------------------------------------------------

void KTouchLectureEditorDialog::saveBtnClicked() {
    if (m_currentURL.isEmpty()) saveAsBtnClicked();
    else {
        transfer_from_dialog();
        m_lecture.saveXML(this, m_currentURL);
        setModified(false);
    }
}
// -----------------------------------------------------------------------------

void KTouchLectureEditorDialog::saveAsBtnClicked() {
    QString tmp = KFileDialog::getSaveFileName(QString(), 
        "*.ktouch.xml|KTouch Lecture Files(*.ktouch.xml)\n*.*|All Files", this, i18n("Save Training Lecture") );
    if (!tmp.isEmpty()) {
        transfer_from_dialog();
        m_currentURL = tmp;
        m_lecture.saveXML(this, m_currentURL);
        setWindowTitle(m_currentURL.url());
        setModified(false);
    }
}
// -----------------------------------------------------------------------------

void KTouchLectureEditorDialog::newSelection(int row) {
    if (row == m_level) return;
    bool current_modified_flag = m_modified;  // remember our current status
    // first store the current level data
    storeCurrentLevel();
    // store the new level number
    m_level = row;
    showCurrentLevel();
    levelListWidget->setCurrentRow(row);

    if (m_lecture.m_lectureData.size()==1) {
        downBtn->setEnabled(false);
        upBtn->setEnabled(false);
    }
    else {
        if (m_level == m_lecture.m_lectureData.size()-1)  
			downBtn->setEnabled(false);
        else
			downBtn->setEnabled(true);
        if (m_level == 0)
			upBtn->setEnabled(false);
        else
			upBtn->setEnabled(true);
    }
    setModified(current_modified_flag);
}
// -----------------------------------------------------------------------------

void KTouchLectureEditorDialog::newCharsChanged(const QString& text) {
	if (m_level >= levelListWidget->count()) return; // paranoia check
	levelListWidget->item(m_level)->setText(text);
    setModified();
}
// -----------------------------------------------------------------------------

void KTouchLectureEditorDialog::newLevel() {
    createNewLevel();
	levelListWidget->addItem(m_lecture.m_lectureData.back().m_newChars);
    newSelection(m_level);
    upBtn->setEnabled(true);
    downBtn->setEnabled(false);
    deleteBtn->setEnabled(true);
    setModified();
}
// -----------------------------------------------------------------------------

void KTouchLectureEditorDialog::deleteLevel() {
    // Open for discussion: Should there be a message box to confirm the delete?
    if (m_level >= m_lecture.m_lectureData.size())
        return; // paranoia check

    // first remove the item from the list widget
    levelListWidget->takeItem(m_level);
    // then remove the level data
    QList<KTouchLevelData>::iterator it=m_lecture.m_lectureData.begin();
    std::advance(it, m_level);
    m_lecture.m_lectureData.erase(it);
    QListWidgetItem *m_currentItem = levelListWidget->item(m_level);
    levelListWidget->setCurrentItem(m_currentItem);
    showCurrentLevel();
    if (m_lecture.m_lectureData.size()==1)
        deleteBtn->setEnabled(false);
    if (m_level==m_lecture.m_lectureData.size()-1)
        downBtn->setEnabled(false);
    if (m_level==0)
        upBtn->setEnabled(false);
    setModified();
}
// -----------------------------------------------------------------------------

void KTouchLectureEditorDialog::moveUp() {
    if (m_level==0)
        return;

    storeCurrentLevel();
    QListWidgetItem *m_currentItem = levelListWidget->currentItem();
    QListWidgetItem *upperItem = levelListWidget->item(m_level-1);
    std::swap(m_lecture.m_lectureData[m_level], m_lecture.m_lectureData[m_level-1]);
    upperItem->setText(m_lecture.m_lectureData[m_level-1].m_newChars);
    m_currentItem->setText(m_lecture.m_lectureData[m_level].m_newChars);
    m_currentItem=upperItem;
    --m_level;
    levelListWidget->setCurrentItem(m_currentItem);
    showCurrentLevel();
    if (m_level==0)
        upBtn->setEnabled(false);
    downBtn->setEnabled(true);
    setModified();
}
// -----------------------------------------------------------------------------

void KTouchLectureEditorDialog::moveDown() {
    if (m_level>=m_lecture.m_lectureData.size()-1)
        return;

    storeCurrentLevel();
    QListWidgetItem *m_currentItem = levelListWidget->currentItem();
    QListWidgetItem *lowerItem = levelListWidget->item(m_level+1);
    std::swap(m_lecture.m_lectureData[m_level], m_lecture.m_lectureData[m_level+1]);
    m_currentItem->setText(m_lecture.m_lectureData[m_level].m_newChars);
    if (lowerItem) {
        lowerItem->setText(m_lecture.m_lectureData[m_level+1].m_newChars);
        m_currentItem=lowerItem;
    }
    ++m_level;
    levelListWidget->setCurrentItem(m_currentItem);
    showCurrentLevel();
    if (m_level==m_lecture.m_lectureData.size()-1)
        downBtn->setEnabled(false);
    upBtn->setEnabled(true);
    setModified();
}
// -----------------------------------------------------------------------------



// ****************************
// ***** Private functions ****
// ****************************

void KTouchLectureEditorDialog::transfer_to_dialog() {
    bool current_modified_flag = m_modified;
    // set the title and the filename of the lecture
    titleEdit->setText(m_lecture.title());
	lectureCommentEdit->setText(m_lecture.m_comment);
    if (m_currentURL.isEmpty()) setWindowTitle(i18n("KTouch Lecture Editor - ") + i18n("<new unnamed lecture file>"));
    else                        setWindowTitle(i18n("KTouch Lecture Editor - ") + m_currentURL.fileName());
    // copy the 'new char' strings of the lectures into the list view
    levelListWidget->clear();
    for (QList<KTouchLevelData>::const_iterator it=m_lecture.m_lectureData.begin();
		it!=m_lecture.m_lectureData.end(); ++it)
	{
    	// add item
		levelListWidget->addItem( it->m_newChars );
	}
    m_level = 0;
    levelListWidget->setCurrentRow(0);
    showCurrentLevel();
    // set up the buttons
    upBtn->setEnabled(false);
    if (m_lecture.m_lectureData.size()>1) {
        downBtn->setEnabled(true);
        deleteBtn->setEnabled(true);
    }
    else {
        downBtn->setEnabled(false);
        deleteBtn->setEnabled(false);
    }
	// finally the font
	if (!m_lecture.m_fontSuggestions.isEmpty()) {
    	QFont f("Monospace");
		// TODO : multiple font suggestions
		f.fromString(m_lecture.m_fontSuggestions);
        linesEdit->setFont(f);
        lectureCommentEdit->setFont(f);
        levelCommentEdit->setFont(f);
        newCharsEdit->setFont(f);
        titleEdit->setFont(f);
        levelListWidget->setFont(f);
    }
    setModified(current_modified_flag);
}
// -----------------------------------------------------------------------------

void KTouchLectureEditorDialog::transfer_from_dialog() {
    storeCurrentLevel();
    m_lecture.m_title = titleEdit->text();
    m_lecture.m_comment = lectureCommentEdit->toPlainText();
    setModified();
}
// -----------------------------------------------------------------------------

void KTouchLectureEditorDialog::showCurrentLevel() {
    if (m_level >= m_lecture.m_lectureData.size())  return;  // should not happen, but why running a risk here...
    levelLabel->setText(i18n("Data of Level %1", m_level+1) );
    levelCommentEdit->setText(m_lecture.m_lectureData[m_level].m_comment);
    newCharsEdit->setText(m_lecture.m_lectureData[m_level].m_newChars);
    QString text;
    for (QList<QString>::const_iterator it=m_lecture.m_lectureData[m_level].m_lines.begin();
                                         it!=m_lecture.m_lectureData[m_level].m_lines.end(); ++it)
    {
        text += *it + '\n';
    }
    linesEdit->setPlainText(text);
}
// -----------------------------------------------------------------------------

void KTouchLectureEditorDialog::storeCurrentLevel() {
    if (m_level>=m_lecture.m_lectureData.size())  return;  // should not happen, but ... better check one time too much...
    m_lecture.m_lectureData[m_level].m_comment = levelCommentEdit->text();
    m_lecture.m_lectureData[m_level].m_newChars = newCharsEdit->text();
    m_lecture.m_lectureData[m_level].m_lines.clear();
    QString text = linesEdit->toPlainText();
    QStringList lines;
    QString currentLine;
    for (int i=0; i<text.length(); ++i) {
        QChar c = text[i];
        if (c=='\t')  c=' '; // replace tabs with spaces
        if (c=='\n') {
            lines.append(currentLine);
            currentLine = "";
        }
        else
            currentLine += c;
    };
    lines.append(currentLine);
    for (QStringList::const_iterator it=lines.begin(); it!=lines.end(); ++it) {
        if ((*it).isEmpty()) continue;
        m_lecture.m_lectureData[m_level].m_lines.push_back(*it);
    }
}
// -----------------------------------------------------------------------------

void KTouchLectureEditorDialog::createNewLevel() {
    KTouchLevelData newLevel;
    newLevel.m_newChars = i18n("abcdefghijklmnopqrstuvwxyz");
    newLevel.m_comment.clear();
    newLevel.m_lines.clear();  // remove the lines of the default mini level
    newLevel.m_lines.push_back(i18n("Enter your lines here..."));
    m_lecture.m_lectureData.push_back(newLevel);
}
// -----------------------------------------------------------------------------

int KTouchLectureEditorDialog::openLectureFile(const KUrl& url) {
    // First setup the open request dialog
    KTouchOpenRequestDialog dlg(this);
    // Call the dialog
    KUrl new_url;
    int result = dlg.requestFileToOpen(new_url,
        i18n("Open Lecture File"),
        i18n("Which Lecture File Would You Like to Edit?"),
        i18n("Edit current lecture:"),
        i18n("Open a default lecture:"),
        i18n("Open a lecture file:"),
        i18n("Create new lecture"),
        url, KTouchPtr->lectureFiles(), i18n("<no lecture files available>"));

    if (result == QDialog::Accepted) {
        // Ok, user confirmed the dialog, now lets get the url
        m_currentURL = new_url;
        // Try to load the lecture, if that failes, we create a new lecture instead
		m_lecture = KTouchLecture();  // empty the lecture
        if (!m_currentURL.isEmpty()) {
			// try to read old format first then XML format
			if (!m_lecture.load(this, m_currentURL) && !m_lecture.loadXML(this, m_currentURL)) {
            	KMessageBox::sorry(this, i18n("Could not open the lecture file, creating a new one instead."));
            	m_currentURL.clear(); // new lectures haven't got a URL
			}
        }
        // If we have no URL, we create a new lecture - can happen if either the user
        // chose "new lecture" or the chosen lecture could not be opened
        if (m_currentURL.isEmpty())  {
            m_lecture.createDefault();
            m_modified = true; // new lectures are modified by default
        }
        else
            m_modified = false; // newly read lectures are not modified in the begin
        // Update our editor with the lecture data        
        transfer_to_dialog();
        return QDialog::Accepted;
    }
    else return QDialog::Rejected;
}
// -----------------------------------------------------------------------------

void KTouchLectureEditorDialog::setModified(bool flag) {
    m_modified = flag;
    if (!m_currentURL.isEmpty()) {
        if (flag) setWindowTitle(i18n("KTouch Lecture Editor - ") + m_currentURL.fileName() + i18n(" (modified)"));
        else      setWindowTitle(i18n("KTouch Lecture Editor - ") + m_currentURL.fileName());
    }
}
// -----------------------------------------------------------------------------

bool KTouchLectureEditorDialog::saveModified() {
    if (!m_modified) return true;
    // ok, ask the user to save the changes
    int result = KMessageBox::questionYesNoCancel(this, 
        i18n("The lecture has been changed. Do you want to save the changes?"),QString(),KStandardGuiItem::save(),KStandardGuiItem::discard());
    if (result == KMessageBox::Cancel) return false; // User aborted
    if (result == KMessageBox::Yes) saveBtnClicked();
    // if successfully saved the modified flag will be reset in the saveBtnClicked() function
    return true; // User acknowledged
}
// -----------------------------------------------------------------------------

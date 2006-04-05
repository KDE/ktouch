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


#include <qlabel.h>
#include <qstring.h>
#include <qstringlist.h>

#include <kpushbutton.h>
#include <klistview.h>
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

#include "ktouchlectureeditor.h"
#include "ktouchlectureeditor.moc"

#include "ktouchlecture.h"
#include "ktouchopenrequest.h"
#include "ktouch.h"


// **************************
// ***** Public functions ***
// **************************

KTouchLectureEditor::KTouchLectureEditor(QWidget *parent, const char* name, bool modal, WFlags fl)
 : KTouchLectureEditorDlg(parent, name, modal, fl)
{
    levelListView->setSorting(-1); // don't sort my level list view!

    connect(levelListView, SIGNAL(selectionChanged(QListViewItem*)),this, SLOT(newSelection(QListViewItem*)) );
    connect(newCharsEdit, SIGNAL(textChanged(const QString&)), this, SLOT(newCharsChanged(const QString&)) );
    connect(newBtn, SIGNAL(clicked()), this, SLOT(newLevel()) );
    connect(deleteBtn, SIGNAL(clicked()), this, SLOT(deleteLevel()) );
    connect(upBtn, SIGNAL(clicked()), this, SLOT(moveUp()) );
    connect(downBtn, SIGNAL(clicked()), this, SLOT(moveDown()) );

    // make the connections for making the lecture modified
    connect(titleEdit, SIGNAL(textChanged(const QString&)), this, SLOT(setModified()) );
    connect(lectureCommentEdit, SIGNAL(textChanged()), this, SLOT(setModified()) );
    connect(levelCommentEdit, SIGNAL(textChanged(const QString&)), this, SLOT(setModified()) );
    connect(linesTextEdit, SIGNAL(textChanged()), this, SLOT(setModified()) );
    
    // The font, open, save, saveas and close buttons are already connected
}
// -----------------------------------------------------------------------------

bool KTouchLectureEditor::startEditor(const KURL& url) {
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

void KTouchLectureEditor::fontBtnClicked() {
    QFont f(m_lecture.m_fontSuggestions);
    if (KFontDialog::getFont(f, false, this, true)==QDialog::Accepted) {
        linesTextEdit->setFont(f);
        lectureCommentEdit->setFont(f);
        levelCommentEdit->setFont(f);
        newCharsEdit->setFont(f);
        titleEdit->setFont(f);
        levelListView->setFont(f);
        m_lecture.m_fontSuggestions = f.family();
        setModified();
    }
}
// -----------------------------------------------------------------------------

void KTouchLectureEditor::openBtnClicked() {
    saveModified();  // save if modified
    openLectureFile("");
}
// -----------------------------------------------------------------------------

void KTouchLectureEditor::saveBtnClicked() {
    if (m_currentURL.isEmpty()) saveAsBtnClicked();
    else {
        transfer_from_dialog();
        m_lecture.saveXML(this, m_currentURL);
        setModified(false);
    }
}
// -----------------------------------------------------------------------------

void KTouchLectureEditor::saveAsBtnClicked() {
    QString tmp = KFileDialog::getSaveFileName(QString::null, 
        "*.ktouch.xml|KTouch Lecture Files(*.ktouch.xml)\n*.*|All Files", this, i18n("Save Training Lecture") );
    if (!tmp.isEmpty()) {
        transfer_from_dialog();
        m_currentURL = tmp;
        m_lecture.saveXML(this, m_currentURL);
        setCaption(m_currentURL.url());
        setModified(false);
    }
}
// -----------------------------------------------------------------------------

void KTouchLectureEditor::newSelection(QListViewItem* item) {
    if (m_selecting) return;
    bool current_modified_flag = m_modified;  // remember our current status
    // first store the current level data
    storeCurrentLevel();
    // now we need to find the level which has been selected
    QListViewItem *i = levelListView->firstChild();
    unsigned int level=0;
    while (i!=0 && i!=item) {
        i = i->nextSibling();
        ++level;
    };
    if (i!=0) {
        m_currentItem = i;
        m_level = level;
        showCurrentLevel();
    }
    m_selecting = true;  // prevent the selectionChanged() signal from interfering
    levelListView->setSelected(m_currentItem, true);
    m_selecting = false;

    if (m_lecture.m_lectureData.size()==1) {
        downBtn->setEnabled(false);
        upBtn->setEnabled(false);
    }
    else {
        if (m_level==m_lecture.m_lectureData.size()-1)  downBtn->setEnabled(false);
        else                            downBtn->setEnabled(true);
        if (m_level==0)                 upBtn->setEnabled(false);
        else                            upBtn->setEnabled(true);
    };
    setModified(current_modified_flag);
}
// -----------------------------------------------------------------------------

void KTouchLectureEditor::newCharsChanged(const QString& text) {
    if (m_currentItem==0) return;  // shouldn't happen, but a little bit of paranoia...
    m_currentItem->setText(0, text);
    setModified();
}
// -----------------------------------------------------------------------------

void KTouchLectureEditor::newLevel() {
    createNewLevel();
    QListViewItem *newItem = new QListViewItem( levelListView, 
        levelListView->lastItem(), m_lecture.m_lectureData.back().m_newChars );
    newSelection(newItem);
    upBtn->setEnabled(true);
    downBtn->setEnabled(false);
    deleteBtn->setEnabled(true);
    setModified();
}
// -----------------------------------------------------------------------------

void KTouchLectureEditor::deleteLevel() {
    // Open for discussion: Should there be a message box to confirm the delete?
    if (m_level >= m_lecture.m_lectureData.size()) return; // paranoia check
    m_selecting = true;  // prevent the selectionChanged() signal from interfering
    // first remove the item from the list view
    delete m_currentItem;
    // then remove the level data
    QValueVector<KTouchLevelData>::iterator it=m_lecture.m_lectureData.begin();
    std::advance(it, m_level);
    m_lecture.m_lectureData.erase(it);
    m_currentItem = levelListView->firstChild();
    for (unsigned int i=0; i<m_level; ++i)
        m_currentItem = m_currentItem->nextSibling();
    levelListView->setSelected(m_currentItem, true);
    showCurrentLevel();
    m_selecting = false;
    if (m_lecture.m_lectureData.size()==1)
        deleteBtn->setEnabled(false);
    if (m_level==m_lecture.m_lectureData.size()-1)
        downBtn->setEnabled(false);
    if (m_level==0)
        upBtn->setEnabled(false);
    setModified();
}
// -----------------------------------------------------------------------------

void KTouchLectureEditor::moveUp() {
    if (m_level==0) return;
    m_selecting=true;  // again, I don't want to process changeSelection() signals now
    storeCurrentLevel();
    QListViewItem *upperItem = m_currentItem->itemAbove();
    std::swap(m_lecture.m_lectureData[m_level], m_lecture.m_lectureData[m_level-1]);
    upperItem->setText(0, m_lecture.m_lectureData[m_level-1].m_newChars);
    m_currentItem->setText(0, m_lecture.m_lectureData[m_level].m_newChars);
    m_currentItem=upperItem;
    --m_level;
    levelListView->setSelected(m_currentItem, true);
    showCurrentLevel();
    m_selecting = false;
    if (m_level==0)
        upBtn->setEnabled(false);
    downBtn->setEnabled(true);
    setModified();
}
// -----------------------------------------------------------------------------

void KTouchLectureEditor::moveDown() {
    if (m_level>=m_lecture.m_lectureData.size()-1) return;
    m_selecting=true;  // again, I don't want to process changeSelection() signals now
    storeCurrentLevel();
    QListViewItem *lowerItem = m_currentItem->itemBelow();
    std::swap(m_lecture.m_lectureData[m_level], m_lecture.m_lectureData[m_level+1]);
    m_currentItem->setText(0, m_lecture.m_lectureData[m_level].m_newChars);
    lowerItem->setText(0, m_lecture.m_lectureData[m_level+1].m_newChars);
    m_currentItem=lowerItem;
    ++m_level;
    levelListView->setSelected(m_currentItem, true);
    showCurrentLevel();
    m_selecting = false;
    if (m_level==m_lecture.m_lectureData.size()-1)
        downBtn->setEnabled(false);
    upBtn->setEnabled(true);
    setModified();
}
// -----------------------------------------------------------------------------



// ****************************
// ***** Private functions ****
// ****************************

void KTouchLectureEditor::transfer_to_dialog() {
    bool current_modified_flag = m_modified;
    // set the title and the filename of the lecture
    titleEdit->setText(m_lecture.title());
	lectureCommentEdit->setText(m_lecture.m_comment);
    if (m_currentURL.isEmpty()) setCaption(i18n("KTouch Lecture Editor - ") + i18n("<new unnamed lecture file>"));
    else                        setCaption(i18n("KTouch Lecture Editor - ") + m_currentURL.fileName());
    // copy the 'new char' strings of the lectures into the list view
    levelListView->clear();
    QValueVector<KTouchLevelData>::const_iterator it=m_lecture.m_lectureData.begin();
    // add first item
    QListViewItem *item=new QListViewItem( levelListView, (it++)->m_newChars );
    // add all the others
    for (;it!=m_lecture.m_lectureData.end(); ++it)
        item = new QListViewItem( levelListView, item, it->m_newChars );
    m_currentItem=levelListView->firstChild();
    m_selecting = true;  // prevent the selectionChanged() signal from interfering
    levelListView->setSelected(m_currentItem, true);
    m_selecting = false;
    // open first lesson
    m_level=0;
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
        linesTextEdit->setFont(f);
        lectureCommentEdit->setFont(f);
        levelCommentEdit->setFont(f);
        newCharsEdit->setFont(f);
        titleEdit->setFont(f);
        levelListView->setFont(f);
    }
    setModified(current_modified_flag);
}
// -----------------------------------------------------------------------------

void KTouchLectureEditor::transfer_from_dialog() {
    storeCurrentLevel();
    m_lecture.m_title = titleEdit->text();
    m_lecture.m_comment = lectureCommentEdit->text();
    setModified();
}
// -----------------------------------------------------------------------------

void KTouchLectureEditor::showCurrentLevel() {
    if (m_level >= m_lecture.m_lectureData.size())  return;  // should not happen, but why running a risk here...
    levelLabel->setText(i18n("Data of Level %1").arg(m_level+1) );
    levelCommentEdit->setText(m_lecture.m_lectureData[m_level].m_comment);
    newCharsEdit->setText(m_lecture.m_lectureData[m_level].m_newChars);
    QString text;
    for (QValueVector<QString>::const_iterator it=m_lecture.m_lectureData[m_level].m_lines.begin();
                                               it!=m_lecture.m_lectureData[m_level].m_lines.end(); ++it)
    {
        text += *it + '\n';
    }
    linesTextEdit->setText(text);
}
// -----------------------------------------------------------------------------

void KTouchLectureEditor::storeCurrentLevel() {
    if (m_level>=m_lecture.m_lectureData.size())  return;  // should not happen, but ... better check one time too much...
    m_lecture.m_lectureData[m_level].m_comment = levelCommentEdit->text();
    m_lecture.m_lectureData[m_level].m_newChars = newCharsEdit->text();
    m_lecture.m_lectureData[m_level].m_lines.clear();
    QString text = linesTextEdit->text();
    QStringList lines;
    QString currentLine;
    for (unsigned int i=0; i<text.length(); ++i) {
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

void KTouchLectureEditor::createNewLevel() {
     KTouchLevelData newLevel;
     newLevel.m_newChars = i18n("abcdefghijklmnopqrstuvwxyz");
     newLevel.m_comment = QString();
     newLevel.m_lines.clear();  // remove the lines of the default mini level
     newLevel.m_lines.push_back(i18n("Enter your lines here..."));
     m_lecture.m_lectureData.push_back(newLevel);
}
// -----------------------------------------------------------------------------

int KTouchLectureEditor::openLectureFile(const KURL& url) {
    // First setup the open request dialog
    KTouchOpenRequest dlg(this);
    // Call the dialog
    KURL new_url;
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
            	m_currentURL = QString::null; // new lectures haven't got a URL
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

void KTouchLectureEditor::setModified(bool flag) {
    m_modified = flag;
    if (!m_currentURL.isEmpty()) {
        if (flag) setCaption(i18n("KTouch Lecture Editor - ") + m_currentURL.fileName() + i18n(" (modified)"));
        else      setCaption(i18n("KTouch Lecture Editor - ") + m_currentURL.fileName());
    }        
}
// -----------------------------------------------------------------------------

bool KTouchLectureEditor::saveModified() {
    if (!m_modified) return true;
    // ok, ask the user to save the changes
    int result = KMessageBox::questionYesNoCancel(this, 
        i18n("The lecture has been changed. Do you want to save the changes?"),QString::null,KStdGuiItem::save(),KStdGuiItem::discard());
    if (result == KMessageBox::Cancel) return false; // User aborted
    if (result == KMessageBox::Yes) saveBtnClicked();
    // if successfully saved the modified flag will be resetted in the saveBtnClicked() function
    return true; // User acknowledged
}
// -----------------------------------------------------------------------------

/***************************************************************************
 *   ktoucheditor.cpp                                                      *
 *   ----------------                                                      *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktoucheditor.h"
#include "ktoucheditor.moc"

#include <qlabel.h>
#include <qstring.h>

#include <kpushbutton.h>
#include <klistview.h>
#include <klineedit.h>
#include <ktextedit.h>
#include <klocale.h>
#include <kdebug.h>

#include <algorithm>  // for std::advance

#include "ktouchlecture.h"

KTouchEditor::KTouchEditor(QWidget *parent, KTouchLecture *lecture)
 : KTouchEditorLayout(parent), m_lecture(lecture)
{
    levelListView->setSorting(-1); // don't sort my level list view!

    connect(levelListView, SIGNAL(selectionChanged(QListViewItem*)),this, SLOT(newSelection(QListViewItem*)) );
    connect(newCharsEdit, SIGNAL(textChanged(const QString&)), this, SLOT(newCharsChanged(const QString&)) );
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()) );
    connect(okButton, SIGNAL(clicked()), this, SLOT(okClicked()) );
    connect(applyButton, SIGNAL(clicked()), this, SLOT(applyClicked()) );

    connect(newBtn, SIGNAL(clicked()), this, SLOT(newLevel()) );
    connect(deleteBtn, SIGNAL(clicked()), this, SLOT(deleteLevel()) );
    connect(upBtn, SIGNAL(clicked()), this, SLOT(moveUp()) );
    connect(downBtn, SIGNAL(clicked()), this, SLOT(moveDown()) );

    // make the connections for enabling the apply button
    connect(commentEdit, SIGNAL(textChanged(const QString&)), this, SLOT(setModified()) );
    connect(linesTextEdit, SIGNAL(textChanged()), this, SLOT(setModified()) );
}


void KTouchEditor::update(bool toDialog) {
    if (toDialog) {
        // first create a local copy of the lecture data, so that we don't have
        // to modify the current lecture and store the current level. So we can
        // revert everything using the cancel button
        m_data=m_lecture->m_lectureData;
        // copy the new char strings of the lectures into the list view
        levelListView->clear();
        if (m_data.count()==0)
            createNewLevel();   // create at least one level in the lecture
        QValueVector<KTouchLevelData>::const_iterator it=m_data.begin();
        // add first item
        QListViewItem *item=new QListViewItem( levelListView, (it++)->m_newChars );
        // add all the others
        for (;it!=m_data.end(); ++it)
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
        if (m_data.count()>1) {
            downBtn->setEnabled(true);
            deleteBtn->setEnabled(true);
        }
        else {
            downBtn->setEnabled(false);
            deleteBtn->setEnabled(false);
        };
    }
    else {
        storeCurrentLevel();
        m_lecture->m_lectureData = m_data;
        m_lecture->m_modified = true;
    };
    applyButton->setEnabled(false);
}


void KTouchEditor::setModified() {
    applyButton->setEnabled(true);
}


void KTouchEditor::newSelection(QListViewItem* item) {
    if (m_selecting) return;
    bool currentApplyState = applyButton->isEnabled();
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
    applyButton->setEnabled(currentApplyState);

    if (m_data.count()==1) {
        downBtn->setEnabled(false);
        upBtn->setEnabled(false);
    }
    else {
        if (m_level==m_data.count()-1)  downBtn->setEnabled(false);
        else                            downBtn->setEnabled(true);
        if (m_level==0)                 upBtn->setEnabled(false);
        else                            upBtn->setEnabled(true);
    };
}


void KTouchEditor::newCharsChanged(const QString& text) {
    if (m_currentItem==0) return;  // shouldn't happen, but a little bit of paranoia...
    m_currentItem->setText(0, text);
    setModified();
}


void KTouchEditor::okClicked() {
    update(false);
    accept();
}


void KTouchEditor::applyClicked() {
    update(false);
}


void KTouchEditor::newLevel() {
    createNewLevel();
    QListViewItem *newItem = new QListViewItem( levelListView, levelListView->lastItem(), m_data.last().m_newChars );
    newSelection(newItem);
    setModified();
    upBtn->setEnabled(true);
    downBtn->setEnabled(false);
    deleteBtn->setEnabled(true);
}


void KTouchEditor::deleteLevel() {
    // Open for discussion: Should there be a message box to confirm the delete?
    // We have a cancel button feature and an apply button... so why ask?

    m_selecting = true;  // prevent the selectionChanged() signal from interfering
    // first remove the item from the list view
    delete m_currentItem;
    // then remove the level data
    QValueVector<KTouchLevelData>::iterator it=m_data.begin();
    std::advance(it, m_level);
    m_data.erase(it);
    if (m_level >= m_data.count())
        --m_level;
    m_currentItem = levelListView->firstChild();
    for (unsigned int i=0; i<m_level; ++i)
        m_currentItem = m_currentItem->nextSibling();
    levelListView->setSelected(m_currentItem, true);
    showCurrentLevel();
    m_selecting = false;
    if (m_data.count()==1)
        deleteBtn->setEnabled(false);
    if (m_level==m_data.count()-1)
        downBtn->setEnabled(false);
    if (m_level==0)
        upBtn->setEnabled(false);
}


void KTouchEditor::moveUp() {
    if (m_level==0) return;
    m_selecting=true;  // again, I don't want to process changeSelection() signals now
    storeCurrentLevel();
    QListViewItem *upperItem = m_currentItem->itemAbove();
    qSwap(m_data[m_level], m_data[m_level-1]);
    upperItem->setText(0, m_data[m_level-1].m_newChars);
    m_currentItem->setText(0, m_data[m_level].m_newChars);
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


void KTouchEditor::moveDown() {
    if (m_level>=m_data.count()-1) return;
    m_selecting=true;  // again, I don't want to process changeSelection() signals now
    storeCurrentLevel();
    QListViewItem *lowerItem = m_currentItem->itemBelow();
    qSwap(m_data[m_level], m_data[m_level+1]);
    m_currentItem->setText(0, m_data[m_level].m_newChars);
    lowerItem->setText(0, m_data[m_level+1].m_newChars);
    m_currentItem=lowerItem;
    ++m_level;
    levelListView->setSelected(m_currentItem, true);
    showCurrentLevel();
    m_selecting = false;
    if (m_level==m_data.count()-1)
        downBtn->setEnabled(false);
    upBtn->setEnabled(true);
    setModified();
}


void KTouchEditor::showCurrentLevel() {
    if (m_level>=m_data.count())  return;  // should not happen, but why running a risk here...
    levelLabel->setText(i18n("Level data of level %1").arg(m_level+1) );
    commentEdit->setText(m_data[m_level].m_comment);
    newCharsEdit->setText(m_data[m_level].m_newChars);
    QString text;
    for (QValueVector<QString>::const_iterator it=m_data[m_level].m_lines.begin();
                                               it!=m_data[m_level].m_lines.end(); ++it)
    {
        text += *it + '\n';
    }
    linesTextEdit->setText(text);
}


void KTouchEditor::storeCurrentLevel() {
    if (m_level>=m_data.count())  return;  // should not happen, but ... better check one time too much...
    m_data[m_level].m_comment = commentEdit->text();
    m_data[m_level].m_newChars = newCharsEdit->text();
    m_data[m_level].m_lines.clear();
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
        m_data[m_level].m_lines.append(*it);
    }
}


void KTouchEditor::createNewLevel() {
     KTouchLevelData newLevel;
     newLevel.m_newChars = "abcdefghijklmnopqrstuvwxyz.,:!";
     newLevel.m_comment = i18n("Enter level description here...");
     newLevel.m_lines.clear();  // remove the lines of the default mini level
     newLevel.m_lines.append(i18n("Enter your lines here..."));
     m_data.append(newLevel);
}

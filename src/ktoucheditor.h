/***************************************************************************
 *   ktoucheditor.h                                                        *
 *   --------------                                                        *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHEDITOR_H
#define KTOUCHEDITOR_H


#include "ktoucheditorlayout.h"
#include "ktouchleveldata.h"

class KTouchLecture;

/** This is the KTouch lecture editor dialog.
 *  The lecture dialog works like a normal dialog and provides an Ok, Apply and
 *  Cancel button. Internally a copy of the currently used lecture is made and
 *  modified. The current lecture in the program is only modified when Ok or Apply
 *  is pressed (just as it should be).
 *  The most important function is as always with dialogs the update() member
 *  function. It transfers the current lecture data into the GUI of the editor and
 *  vice versa.<p>
 *  When using the lecture editor dialog you need to pass a pointer to the lecture object
 *  during construction, so a typical usage of your editor dialog would look like:
 *  @code
 *   KTouchLecture my_lecture;
 *  // ... put some stuff in your lecture
 *  KTouchEditor editorDlg(this, &my_lecture);  // create the dialog
 *  editorDlg->update(true);                    // transfer data from my_lecture into the dialog
 *  if (editorDlg->exec()==QDialog::Accepted)   // and finally execute the dialog
 *      editorDlg->update(false);               // transfers data back into my_lecture
 *  @endcode
 *  <p>Warning: When using a global lecture object make sure the pointer you passed to
 *  the dialog during creating is still valid when you execute the dialog!<p>
 */
class KTouchEditor : public KTouchEditorLayout {
    Q_OBJECT
  public:
    /// Constructor, takes a pointer to the current lecture object.
    KTouchEditor(QWidget *parent, KTouchLecture *lecture);
    /// Transfers data from the lecture object to the dialog and vice versa.
    void update(bool toDialog);

  private slots:
    /// Will be called whenever some changes are made.
    void setModified();
    /// Will be called whenever another level has been selected.
    void newSelection(QListViewItem* item);
    /// Will be called whenever another.
    void newCharsChanged(const QString& text);
    /// Will be called when the OK button has been clicked.
    void okClicked();
    /// Will be called when the Apply button has been clicked.
    void applyClicked();
    /// Will be called when the "New Level" button has been clicked.
    void newLevel();
    /// Will be called when the "Delete Level" button has been clicked.
    void deleteLevel();
    /// Will be called when the "Move Level Up" button has been clicked.
    void moveUp();
    /// Will be called when the "Move Level Down" button has been clicked.
    void moveDown();

  private:
    /// Fills the level user interface with data from the current level (m_level).
    void showCurrentLevel();
    /// Copies the content of the user interface into the vector with the level data.
    void storeCurrentLevel();
    /// Creates a default level and appends it to the already existing levels.
    void createNewLevel();

    KTouchLecture                  *m_lecture;         ///< a pointer to the currently used lecture
    QValueVector<KTouchLevelData>   m_data;            ///< a copy of the lecture data for the editor
    unsigned int                    m_level;           ///< the current level number
    QListViewItem                  *m_currentItem;     ///< the currently selected item in the list view
    bool                            m_selecting;       ///< flag to prevent the selection slot from selecting twice
};

#endif

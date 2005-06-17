/***************************************************************************
 *   ktouchkeyboardeditor.h                                                *
 *   ----------------------                                                *
 *   Copyright (C) 2000 by Håvard Frøiland, 2004 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net, ghorwin@users.sourceforge.net          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHKEYBOARDEDITOR_H
#define KTOUCHKEYBOARDEDITOR_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ktouchkeyboardwidget.h"
#include "ktouchkeyboardeditor_dlg.h"

#include "ktouchkeyboard.h"

#include <kurl.h>

/// This is the KTouch keyboard editor dialog.
///
/// The keyboard edit dialog works like a standalone edit tool for editing keyboards.
/// Upon opening of the dialog the 'open keyboard file' dialog box is shown where
/// the user can either select a local keyboard file or load a keyboard file from
/// somewhere. Then the user can edit the keyboard and save it (if desired under a 
/// different file name).
/// The use of the editor is simple, just include ktouchkeyboardeditor.h and do
/// @code
///  KTouchKeyboardEditor dlg;
///  bool ok = dlg.startEditor(current_keyboard);
///  // current_keyboard should be the URL pointing to the current/default 
///  // keyboard or an empty URL
/// @endcode
/// @see startEditor() for details on the return value.
class KTouchKeyboardEditor : public KTouchKeyboardEditorDlg {
    Q_OBJECT

  public:
    /// Constructor.
    KTouchKeyboardEditor(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    /// Default destructor.
    ~KTouchKeyboardEditor() {}
    /// Shows and executes the dialog (argument is the url to the default or 
    /// current keyboard file).
    /// @return Returns 'true' if the dialog was properly executed (thus reloading 
    ///         of the keyboard in KTouch is necessary) or 'false' if user canceled
    ///         the "Open request" dialog.
    bool startEditor(const KURL& url);

  public slots:
    /// Called when the Font button was clicked.
    virtual void fontBtnClicked();
    /// Called when the Open button was clicked.
    virtual void openBtnClicked();
    /// Called when the Save button was clicked.
    virtual void saveBtnClicked();
    /// Called when the Save As button was clicked.
    virtual void saveAsBtnClicked();
    /// Called when the editor is closed.
    virtual void closeQuery() { if (saveModified()) accept(); };
    /// Called when the "Add..." button was clicked
    virtual void addBtnClicked();
    /// Called when the "Edit..." button was clicked
    virtual void editBtnClicked();
    /// Called when the "Remove" button was clicked
    virtual void removeBtnClicked();
    /// Called when the selection in the key list box has changed
	virtual void keySelectionChanged(QListBoxItem * item);
    /// Will be called whenever some changes are made.
    void setModified() { setModified(true); }

  protected slots:
    /// Called when the editor is rejected (x clicked).
    void reject() { closeQuery(); };


  protected:
    /// Draws the keyboard in the keyboardPreviewLabel.
    void paintEvent(QPaintEvent *);
  
  private:    
    /// Transfers data from the keyboard object to the dialog.
    void transfer_to_dialog();
    /// Transfers data from the dialog to the keyboard object.
    void transfer_from_dialog();
    /// Setups and opens the 'Open request dialog'. The url 'url' is used in the
    /// open request dialog for the 'current' url selection. When the open request 
    /// dialog is closed the function attempts to load the keyboard or if that failes
    /// creates a new, empty keyboard. 
    /// @return The function returns the return code from the open request dialog, so 
    ///         that one can react on a "cancel" choice.
    int openKeyboardFile(const KURL& url);
    /// Changes the state of the lecture file (flag==true means modified).
    void setModified(bool flag);
    /// If the keyboard is modified the user is prompted to save it or throw away the changes. 
    /// If the user accepts the dialog the file is saved. 
    /// @return The function returns 'false' if the user aborted the save request, otherwise true.
    bool saveModified();
   
    KTouchKeyboard	m_keyboard;        ///< The keyboard data.
    bool            m_selecting;       ///< Flag to prevent the selection slot from being selected twice.
    bool            m_modified;        ///< Flag indicating whether the keyboard has been modified.
    KURL            m_currentURL;      ///< URL of the current keyboard.
};

#endif   // KTOUCHKEYBOARDEDITOR_H

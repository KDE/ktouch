/***************************************************************************
 *   ktouchkeyboardeditordialog.h                                          *
 *   ----------------------------                                          *
 *   Copyright (C) 2000-2007 by Håvard Frøiland and Andreas Nicolai        *
 *   haavard@users.sourceforge.net, ghorwin@users.sourceforge.net          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHKEYBOARDEDITORDIALOG_H
#define KTOUCHKEYBOARDEDITORDIALOG_H

#include <QDialog>
#include <QGraphicsView>
#include <QResizeEvent>

#include <kurl.h>

#include "ui_ktouchkeyboardeditordialog.h"

#include "ktouchkeyboard.h"

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
class KTouchKeyboardEditorDialog : public QDialog, public Ui_KTouchKeyboardEditorDialog {
	Q_OBJECT

  public:
    /// Constructor.
    KTouchKeyboardEditorDialog(QWidget* parent = 0, Qt::WFlags fl = 0 );
    /// Default destructor.
    virtual ~KTouchKeyboardEditorDialog() {}
    /// Shows and executes the dialog (argument is the url to the default or 
    /// current keyboard file).
    /// @return Returns 'true' if the dialog was properly executed (thus reloading 
    ///         of the keyboard in KTouch is necessary) or 'false' if user canceled
    ///         the "Open request" dialog.
    bool startEditor(const KUrl& url);

  public slots:
    /// Called when the Font button was clicked.
    virtual void on_setFontButton_clicked();
    /// Called when the Open button was clicked.
    virtual void on_openButton_clicked();
    /// Called when the Save button was clicked.
    virtual void on_saveButton_clicked();
    /// Called when the Save As button was clicked.
    virtual void on_saveAsButton_clicked();
    /// Called when the editor is closed.
    virtual void closeQuery() { if (saveModified()) accept(); }
	/// Called when user edits the top left character
	virtual void on_topLeftChar_textEdited(const QString & text); 
	/// Called when user edits the top right character
	virtual void on_topRightChar_textEdited(const QString & text);
	/// Called when user edits the bottom left character
	virtual void on_bottomLeftChar_textEdited(const QString & text);
	/// Called when user edits the bottom right character
	virtual void on_bottomRightChar_textEdited(const QString & text);
	/// Called when user selects a keytype.
	virtual void on_keyTypeCombo_currentIndexChanged(int index);
	/// Called when user edits the key text
	virtual void on_keyTextEdit_textEdited(const QString & text);
	/// Called when user deletes a key.
	virtual void on_deleteKeyButton_clicked(bool);
	/// Called when user adds a new key.
	virtual void on_addKeyButton_clicked(bool);
	/// Called when user adds a connector
	virtual void on_addConnectorButton_clicked(bool);
	/// Called when user clears all connectors for a certain key.
	virtual void on_clearConnectorButton_clicked(bool);

	/// Called whenever the user modified the key geometry.
	virtual void on_leftSpinBox_valueChanged(int);
	virtual void on_topSpinBox_valueChanged(int);
	virtual void on_widthSpinBox_valueChanged(int);
	virtual void on_heightSpinBox_valueChanged(int);

    /// Will be called whenever some changes are made.
    void setModified() { setModified(true); }
    /// Resizes the keyboard based on the size of the graphics view.
    void resizeKeyboard();
	/// Connected to the key-graphicsview item 'clicked' event.
	void keyClicked(KTouchKey * k);
	/// Connected to the key-graphicsview items 'moved' event.
	void keyPositionChanged(KTouchKey * k);

  protected:
    /// Resizes the keyboard.
    void resizeEvent(QResizeEvent *);

  protected slots:
    /// Called when the editor is rejected (x clicked).
    void reject() { closeQuery(); }

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
    int openKeyboardFile(const KUrl& url);
    /// Changes the state of the lecture file (flag==true means modified).
    void setModified(bool flag);
    /// If the keyboard is modified the user is prompted to save it or throw away the changes. 
    /// If the user accepts the dialog the file is saved. 
    /// @return The function returns 'false' if the user aborted the save request, otherwise true.
    bool saveModified();

    KTouchKeyboard	*m_keyboard;		///< The keyboard data.
    bool            m_modified;			///< Flag indicating whether the keyboard has been modified.
    KUrl            m_currentURL;		///< URL of the current keyboard.

	KTouchKey		*m_currentEditKey;	///< The key that is currently selected for editing.

    QGraphicsScene	*m_scene;			///< The graphics scene 

};

#endif   // KTOUCHKEYBOARDEDITORDIALOG_H

/***************************************************************************
 *   ktouch.cpp                                                            *
 *   ----------                                                            *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef _KTOUCH_H_
#define _KTOUCH_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kmainwindow.h>
#include <kapplication.h>
#include <kurl.h>

class QLabel;
class KToggleAction;
class KActionMenu;

class KTouchLecture;
class KTouchPref;
class KTouchEditor;
class KTouchStatistics;
class KTouchStatus;
class KTouchSlideLine;
class KTouchKeyboard;
class KTouchTrainer;
class KTouchStartNewDialog;

/** This is the main window of KTouch.
 *  It handles the lecture, training data, status and all other widgets that are
 *  needed to get the program running. Since all special tasks are delegated to the
 *  appropriate widgets and classes, the remaining code in KTouch is basically the
 *  startup and KAction stuff.<p>
 *  A word about dialogs. Although we have only one preferences and one editor dialogs
 *  we don't create them by default. Instead they are create "on first use". This
 *  saves memory (because we don't need them always) and the startup speed increases.<p>
 *  One central function - the keyPressEvent() - is responsable for getting the actual
 *  typed char. But it does nothing else then delegating the character to the trainer
 *  (KTouchTrainer), which will then process it. So the heavy work lies in the trainer
 *  object and all the widgets.
 */
class KTouch : public KMainWindow {
    Q_OBJECT
  public:
    /// Constructor, creates the KTouch proggy.
    KTouch();
    /// Destructor, frees allocated memory for dialogs and stuff like that.
    virtual ~KTouch();

  public slots:
    /// Will be called when the "Apply"-button has been pressed in the preferences
    /// dialog or when the user accepted the changes using the "OK"-button.
    void applyPreferences();
    /// Updates the caption of the main window to ("KTouch - filename").
    void updateCaption();
    /// Accepts a typed char.
    void keyPressEvent(QKeyEvent *keyEvent);

  protected:
    /// Reimplementated to ask user to save his changed lecture data.
    bool queryClose();
    /// Reimplementated to save preferences and
    bool queryExit();

  private slots:
    void fileNew();                     ///< The action File->New
    void fileOpen();                    ///< The action File->Open...
    bool fileSave();                    ///< The action File->Save
    void fileSaveAs();                  ///< The action File->SaveAs...
    void fileQuit();                    ///< The action File->Quit
    void trainingNewSession();          ///< The action Training->Start new training session...
    void trainingContinue();            ///< The action Training->Continue training
    void trainingPause();               ///< The action Training->Pause training
    void trainingStatistics();          ///< The action Training->Show training statistics...
    void trainingLectureEdit();         ///< The action Training->Edit lecture...
    void optionsShowToolbar();          ///< The action Settings->Show toolbar
    void optionsShowStatusbar();        ///< The action Settings->Show statusbar
    void optionsPreferences();          ///< The action Settings->Configure KTouch...

    /// Updates the status bar text.
    void changeStatusbarMessage(const QString& text);
    /// Updates the status bar statistics.
    void changeStatusbarStats(unsigned int correctChars, unsigned int totalChars, unsigned int words);
    /// Quick-changes the keyboard layout.
    void changeKeyboard(int num);
    /// Quick-changes the colour scheme used on the keyboard.
    void changeColor(int num);
    /// Quick-changes the lecture.
    void changeLecture(int num);

    void slotConfigureKeys();

  private:
    /// Will be called when this app is restored.
    void readProperties(KConfig *config);
    /// Will be called when the app should save its state for session management purposes.
    void saveProperties(KConfig *config);
    /// Reads the last used lecture URL and the training session data of the last session.
    void readTrainingState(KConfig *config);
    /// Saves the last used lecture URL and the training session data of the last session.
    void saveTrainingState(KConfig *config);
    /// Reloads the lecture of the last session (during normal startup).
    void reloadLecture();
    /// Creates the (standard) actions.
    void setupActions();
    /// Appends submenues in the settings menu for the default keyboard layouts and standard lectures.
    void setupQuickSettings();
    /// If the current lecture was modified the user is asked to save it.
    /// Returns 'false' if the user aborted the save request, otherwise 'true'.
    bool saveModified();

    KToggleAction          *m_toolbarAction;        ///< Action for turning the toolbar on/off.
    KToggleAction          *m_statusbarAction;      ///< Action for turning the status bar on/off.
    KAction                *m_trainingContinue;     ///< Action for "continue training session".
    KAction                *m_trainingPause;        ///< Action for "pause training session".

    KTouchPref             *m_preferencesDlg;       ///< Pointer to the preferences dialog.
    KTouchEditor           *m_editorDlg;            ///< Pointer to the lecture editor dialog.
    KTouchStartNewDialog   *m_startNewDlg;          ///< Pointer to the "start new session" dialog.
    KTouchStatus           *m_statusWidget;         ///< Pointer to the status widget on top of the main widget.
    KTouchSlideLine        *m_slideLineWidget;      ///< Pointer to the sliding line widget.
    KTouchKeyboard         *m_keyboardWidget;       ///< Pointer to the keyboard widget.
    KTouchStatistics       *m_statsWidget;          ///< Pointer to the statistics (dialog) widget.
    KTouchLecture          *m_lecture;              ///< Pointer to the lecture data.
    KTouchTrainer          *m_trainer;              ///< The training 'master' (runs the training).

    QLabel                 *m_barStatsLabel;        ///< The textlabel displaying the correct number of typed chars.
};

#endif // _KTOUCH_H_

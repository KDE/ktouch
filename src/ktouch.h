/***************************************************************************
 *   ktouch.cpp                                                            *
 *   ----------                                                            *
 *   Copyright (C) 2000 by Håvard Frøiland, 2004 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
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
class KSelectAction;

class KTouchStatus;
class KTouchSlideLine;
class KTouchKeyboard;
class KTouchTrainer;

class KTouchPrefTrainingLayout;
class KTouchPrefKeyboardLayout;
class KTouchPrefGeneralLayout;

#include "ktouchlecture.h"

/// This is the main window of KTouch.
///
/// It handles the lecture, training data, status and all other widgets that are
/// needed to get the program running. Since all special tasks are delegated to the
/// appropriate widgets and classes, the remaining code in KTouch is basically the
/// startup and KAction stuff.<p>
/// A word about dialogs. Although we have only one preferences and one editor dialogs
/// we don't create them by default. Instead they are create "on first use". This
/// saves memory (because we don't need them always) and the startup speed increases.<p>
/// One central function - the keyPressEvent() - is responsable for getting the actual
/// typed char. But it does nothing else then delegating the character to the trainer
/// (KTouchTrainer), which will then process it. So the heavy work lies in the trainer
/// object and all the widgets.
class KTouch : public KMainWindow {
    Q_OBJECT
  public:
    /// Constructor, creates the KTouch proggy.
    KTouch();
    /// Destructor, releases memory of KTouch trainer.
    ~KTouch();

  public slots:
    /// Will be called when the "Apply"-button has been pressed in the preferences
    /// dialog or when the user accepted the changes using the "OK"-button.
    void applyPreferences();
    /// Accepts a typed char.
    void keyPressEvent(QKeyEvent *keyEvent);

	/// Called from the configuration dialog. 
	void configOverrideLectureFontToggled(bool on);
	/// Called from the configuration dialog. 
	void configOverrideKeyboardFontToggled(bool on);
	/// Called from the configuration dialog.
	void configAutoLevelChangeToggled(bool on);

  protected:
    /// Reimplementated to ask user whether he/she wants to abort the test
    bool queryClose();
    /// Reimplementated to save preferences and
    bool queryExit();

  private slots:
    void fileEditLecture();             ///< The action File->Edit lecture...
    void fileQuit();                    ///< The action File->Quit
    void trainingNewSession();          ///< The action Training->Start new training session...
    void trainingContinue();            ///< The action Training->Continue training
    void trainingPause();               ///< The action Training->Pause training
    void trainingStatistics();          ///< The action Training->Show training statistics...
    void optionsPreferences();          ///< The action Settings->Configure KTouch...

    /// Updates the status bar text.
    void changeStatusbarMessage(const QString& text);
    /// Updates the status bar statistics.
    void changeStatusbarStats(unsigned int correctChars, unsigned int totalChars, unsigned int words);
    /// Quick-changes the keyboard layout (called from menu).
    void changeKeyboard(int num);
    /// Quick-changes the colour scheme used on the keyboard (called from menu).
    void changeColor(int num);
    /// Quick-changes the current training lecture file (called from menu).
    void changeLecture(int num);

  private:
    /// Will be called when this app is restored due to session management.
    void readProperties(KConfig *config);
    /// Will be called when the app should save its state for session management purposes.
    void saveProperties(KConfig *config);
    /// Creates the (standard) actions.
    void setupActions();
	/// Call this function whenever you have read a new lecture file to update the slide line widget.
	void updateFontFromLecture();

    KAction                *m_trainingContinue;     ///< Action for "continue training session".
    KAction                *m_trainingPause;        ///< Action for "pause training session".

    KSelectAction		   *m_keyboardLayoutAction;
    KSelectAction		   *m_keyboardColorAction;
    KSelectAction		   *m_defaultLectureAction;

    KTouchStatus           *m_statusWidget;         ///< Pointer to the status widget on top of the main widget.
    KTouchSlideLine        *m_slideLineWidget;      ///< Pointer to the sliding line widget.
    KTouchKeyboard         *m_keyboardWidget;       ///< Pointer to the keyboard widget.
    KTouchTrainer          *m_trainer;              ///< The training 'master' (runs the training).
    KTouchLecture           m_lecture;              ///< The lecture data.

	KTouchPrefGeneralLayout  * m_pageGeneral;		///< The general configuration page.
	KTouchPrefTrainingLayout * m_pageTraining;		///< The training configuration page.
	KTouchPrefKeyboardLayout * m_pageKeyboard;		///< The keyboard configuration page.
	
    QLabel                 *m_barStatsLabel;        ///< The textlabel displaying the correct number of typed chars.
};

#endif // _KTOUCH_H_

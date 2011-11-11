/***************************************************************************
 *   ktouch.h                                                              *
 *   --------                                                              *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003-2007 by Andreas Nicolai   *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCH_H
#define KTOUCH_H

#include <QLabel>
#include <QKeyEvent>
#include <QResizeEvent>

#include <kxmlguiwindow.h>
#include <kurl.h>

class QDeclarativeView;
class KAction;
class KSelectAction;

class KTouchStatusWidget;
class KTouchTextLineWidget;
class KTouchKeyboardWidget;
class KTouchTrainer;

class KTouch : public KXmlGuiWindow {
    Q_OBJECT
  public:
    /// Constructor, creates the KTouch proggy.
    KTouch();
    /// Destructor, releases memory of KTouch trainer.
    ~KTouch();

        /// Returns the available lecture files
        const QStringList& lectureFiles() const { return m_lectureFiles; }
        /// Returns the available keyboard files
        const QStringList& keyboardFiles() const { return m_keyboardFiles; }
        /// Returns the statistics object for the current lecture (as reference)


  protected:
    /// Reimplementated to save preferences and
    bool queryExit();
    /// Some layout fixes here...
    void resizeEvent(QResizeEvent *);

    //void inputMethodEvent( QInputMethodEvent* m );

  private:
    /// Initialises the program during a normal startup
        void init();
        void updateFileLists();
        QDeclarativeView* m_view;

        /// Creates the layout and GUI setup for a practice session
        QStringList     		m_lectureFiles;         ///< A list of all default lecture files.
        QStringList     		m_lectureTitles;        ///< A list of the titles of all default lecture files.
        QString 			m_startLectureDir;	/// Path to standard lecture files. Usually /usr/share/apps/ktouch

            QStringList     		m_examinationFiles;     ///< A list of all default examination files.
            QStringList     		m_examinationTitles;    ///< A list of the titles of all default examination files.

        QStringList     		m_keyboardFiles;        ///< A list of all default keyboard layout files.
        QStringList     		m_keyboardTitles;       ///< A list of the titles of all default keyboard layout files.

};

#endif // KTOUCH_H

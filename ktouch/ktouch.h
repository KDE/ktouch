/***************************************************************************
                          ktouch.h  -  description
                             -------------------
    begin                : Tue Mar 13 15:33:49 EST 2001
    copyright            : (C) 2001 by Haavard Froeiland
    email                : haavard@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef KTOUCH_H
#define KTOUCH_H


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kapp.h>
#include <kmainwindow.h>
#include <kaccel.h>
#include <kaction.h>
#include <kstddirs.h>

#include "touchlecture.h"
#include "touchline.h"
#include "touchStatus.h"
#include "touchkeyboard.h"
#include "optionKeyboard.h"
#include "optionTraining.h"
#include "optionGeneral.h"

class KTouch : public KMainWindow
{
  Q_OBJECT

  public:
    /** construtor of KTouch2App, calls all init functions to create the application.
     */
    KTouch(QWidget* parent=0, const char* name=0);
    TouchLecture *touchLecture;
    TouchLine *touchLine;
    TouchStatus *touchStatus;
  	TouchKeyboard *touchKeyboard;

    /** opens a file specified by commandline option
     */
    void openDocumentFile(const KURL& url=0);

  protected:
    /** save general Options like all bar positions and status as well as the geometry and the recent file list to the configuration
     * file
     */ 	
    void saveOptions();
    /** read general Options again and initialize all variables like the recent file list
     */
    void readOptions();
    /** initializes the KActions of the application
    */
    void initActions();
    /** sets up the statusbar for the main window by initialzing a statuslabel.
     */
    void initStatusBar();
    /** initializes the document object of the main window that is connected to the view in initView().
     * @see initView();
     */

    bool queryExit();
    void keyPressEvent(QKeyEvent*e);


  public slots:
    void slotFileOpen();
    /** opens a file from the recent files menu */
    void slotFileOpenRecent(const KURL& url);

    void slotFileQuit();
    /** put the marked text/object into the clipboard and remove
     *	it from the document
     */
    void slotViewToolBar();
    /** toggles the statusbar
     */
    void slotViewStatusBar();
    /** changes the statusbar contents for the standard label permanently, used to indicate current actions.
     * @param text the text that is displayed in the statusbar
     */
    void slotStatusMsg(const QString &text);

    void slotOptionKeyboard();
    void slotOptionTraining();
    void slotOptionGeneral();

  private:
    /** the configuration object of the application */
    KConfig *config;
    /** view is the main widget which represents your working area. The View
     * class should handle all events of the view widget.  It is kept empty so
     * you can create your view according to your application's needs by
     * changing the view class.
     */

    // KAction pointers to enable/disable actions
    KAction* fileOpen;
    KRecentFilesAction* fileOpenRecent;
    KAction* fileClose;
    KAction* fileQuit;
    KToggleAction* viewToolBar;
	  KToggleAction* viewStatusBar;

    KStandardDirs *dirs;
    bool remember;
};

#endif


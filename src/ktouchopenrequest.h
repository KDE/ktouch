/***************************************************************************
 *   ktouchopenrequest.h                                                   *
 *   -------------------                                                   *
 *   Copyright (C) 2004 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHOPENREQUEST_H
#define KTOUCHOPENREQUEST_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ktouchopenrequest_dlg.h"

#include <kurl.h>
#include <tqstringlist.h>

/// Dialog for selecting a file to open.
///
/// This dialog is a costumizable dialog and is to be used for all editors.
/// It offers the choice between a default (current) file, a prepared selection of 
/// files, choosing an arbitrary file from somewhere or creating a new file.
/// Before using the dialog you have to set it up. You can do this using the
/// all-in-one function requestFileToOpen() or manually by setting the 
/// different widgets yourself. Usually the first proposal will be best, as 
/// in the following example:
/// @code
/// KTouchOpenRequest dlg;
/// KURL url;
/// // current_lecture must hold the URL to the current/default lecture or ""
/// // default_lectures must be a TQStringList with the default lecture files.
/// int result = dlg.requestFileToOpen(url, i18n("Open a lecture file"), 
///     i18n("Which lecture would you like to open?"), i18n("Current lecture:"), 
///     i18n("Open a default lecture:"), i18n("Open a lecture file:"), 
///     i18n("Create a new lecture!"), current_lecture, 
///     default_lectures, i18n("<no lecture files available>"));
///
/// if (result==TQDialog::Accepted) {
///     doSomethingFancyWith(url);
/// }
/// @endcode
/// If you don't use the function and execute the dialog manually, the chosen
/// url is stored in the member variable m_url (if the dialog was accepted by 
/// the user).
/// 
class KTouchOpenRequest : public KTouchOpenRequestDlg {
  Q_OBJECT
 
  public:
    /// Default constructor.
    KTouchOpenRequest(TQWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    /// Default destructor.
    ~KTouchOpenRequest() {}
    /// Sets up the dialog and runs it.
    /// @return Returns the dialogs return code, TQDialog::Accepted or TQDialog::Rejected.
    int requestFileToOpen(KURL& url, const TQString& caption, const TQString& title, const TQString& currentText,
        const TQString& defaultText, const TQString& openText, const TQString& newText,
        KURL current_url = TQString::null, TQStringList defaultList = TQStringList(), TQString emptyListText = TQString::null);

    /// After the dialog has been accepted the URL chosen by the user is stored herein.
    KURL m_url;    
     
  public slots:
    /// Called when user selects Ok (used to check whether the URL is well formed).
    void okBtnClicked();
    /// Called when user switches to another radiobutton.
    void radioBtnChanged();
    /// Called when the user clicks on the "Browse" button and opens the file select dialog.
    void browseBtnClicked();

};

#endif


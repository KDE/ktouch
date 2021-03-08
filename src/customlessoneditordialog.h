/*
 * SPDX-FileCopyrightText: 2013 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */


#ifndef CUSTOMLESSONEDITORDIALOG_H
#define CUSTOMLESSONEDITORDIALOG_H

#include <QDialog>

#include "ui_customlessoneditordialog.h"

class Lesson;
class KeyboardLayout;

class CustomLessonEditorDialog : public QDialog, private Ui::CustomLessonEditorDialog
{
    Q_OBJECT
public:
    explicit CustomLessonEditorDialog(QWidget* parent = 0);
    Lesson* lesson() const;
    void setLesson(Lesson* lesson);
    KeyboardLayout* keyboardLayout() const;
    void setKeyboardLayout(KeyboardLayout* keyboardLayout);
signals:
    void validChanged(bool valid);
private slots:
    void updateTitle();
    void updateText();
    void updateAllowedCharacters();
    void onTitleChanged();
    void onTextChanged();
private:
    void emitValidChanged();
    Lesson* m_lesson;
    KeyboardLayout* m_keyboardLayout;
};

#endif // CUSTOMLESSONEDITORDIALOG_H

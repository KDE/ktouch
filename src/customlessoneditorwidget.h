/*
 * Copyright 2013  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef CUSTOMLESSONEDITORWIDGET_H
#define CUSTOMLESSONEDITORWIDGET_H

#include <QWidget>

#include "ui_customlessoneditorwidget.h"

class Lesson;
class KeyboardLayout;

class CustomLessonEditorWidget : public QWidget, private Ui::CustomLessonEditorWidget
{
    Q_OBJECT
public:
    explicit CustomLessonEditorWidget(QWidget* parent = 0);
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

#endif // CUSTOMLESSONEDITORWIDGET_H

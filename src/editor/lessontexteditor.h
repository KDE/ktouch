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

#ifndef LESSONTEXTEDITOR_H
#define LESSONTEXTEDITOR_H

#include "ui_lessontexteditor.h"

class KTextEdit;
class LessonTextHighlighter;

class LessonTextEditor : public QWidget, private Ui::LessonTextEditor
{
    Q_OBJECT
public:
    explicit LessonTextEditor(QWidget* parent = 0);
    bool readOnly() const;
    void setReadOnly(bool readOnly);
    KTextEdit* textEdit() const;
    LessonTextHighlighter* highlighter() const;
private slots:
    void onLessonTextChanged();
    void openTextFile();
    void reformatText();
private:
    bool m_readOnly;
    LessonTextHighlighter* m_highlighter;
};

#endif // LESSONTEXTEDITOR_H

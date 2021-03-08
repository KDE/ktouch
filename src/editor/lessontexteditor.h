/*
 * SPDX-FileCopyrightText: 2013 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
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

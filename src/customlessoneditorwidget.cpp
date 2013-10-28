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

#include "customlessoneditorwidget.h"

#include "core/lesson.h"

CustomLessonEditorWidget::CustomLessonEditorWidget(QWidget* parent) :
    QWidget(parent),
    Ui::CustomLessonEditorWidget(),
    m_lesson(0)
{
    this->setupUi(this);

    connect(m_titleLineEdit, SIGNAL(textChanged(QString)), SLOT(onTitleChanged()));
    connect(m_lessonTextEditor->textEdit(), SIGNAL(textChanged()), SLOT(onTextChanged()));
}

Lesson* CustomLessonEditorWidget::lesson() const
{
    return m_lesson;
}

void CustomLessonEditorWidget::setLesson(Lesson* lesson)
{
    if (lesson != m_lesson)
    {
        if (m_lesson)
        {
            m_lesson->disconnect(this);
        }

        m_lesson = lesson;

        if (lesson)
        {
            updateTitle();
            updateText();

            connect(lesson, SIGNAL(titleChanged()), SLOT(updateTitle()));
            connect(lesson, SIGNAL(textChanged()), SLOT(updateText()));
        }
    }
}

void CustomLessonEditorWidget::updateTitle()
{
    const QString title = m_lesson->title();

    if (title != m_titleLineEdit->text())
    {
        m_titleLineEdit->setText(title);
    }
}

void CustomLessonEditorWidget::updateText()
{
    const QString text = m_lesson->text();

    if (text != m_lessonTextEditor->textEdit()->toPlainText())
    {
        m_lessonTextEditor->textEdit()->setPlainText(text);
    }
}

void CustomLessonEditorWidget::onTitleChanged()
{
    if (!m_lesson)
        return;

    m_lesson->setTitle(m_titleLineEdit->text());

    emitValidChanged();
}

void CustomLessonEditorWidget::onTextChanged()
{
    if (!m_lesson)
        return;

    m_lesson->setText(m_lessonTextEditor->textEdit()->toPlainText());

    emitValidChanged();
}

void CustomLessonEditorWidget::emitValidChanged()
{
    const QString title = m_lesson->title();
    const QString text = m_lesson->text();
    const bool valid = !title.isEmpty() && !text.isEmpty();

    emit validChanged(valid);
}

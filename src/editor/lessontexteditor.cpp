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

#include "lessontexteditor.h"

#include <QTextCursor>
#include <QTextDocumentFragment>

#include <KFileDialog>
#include <KMessageBox>
#include <KDialog>

#include "editor/lessontexthighlighter.h"

LessonTextEditor::LessonTextEditor(QWidget* parent) :
    QWidget(parent),
    Ui::LessonTextEditor(),
    m_readOnly(false),
    m_highlighter(new LessonTextHighlighter(this))
{
    setupUi(this);
    m_highlighter->setMaximumLineLength(60);
    m_highlighter->setDocument(m_lessonTextEdit->document());
    connect(m_openTextFileButton, SIGNAL(clicked()), SLOT(openTextFile()));
    connect(m_reformatLessonTextButton, SIGNAL(clicked()), SLOT(reformatText()));
    connect(m_lessonTextEdit, SIGNAL(textChanged()), SLOT(onLessonTextChanged()));
}

bool LessonTextEditor::readOnly() const
{
    return m_readOnly;
}

void LessonTextEditor::setReadOnly(bool readOnly)
{
    if (readOnly != m_readOnly)
    {
        m_readOnly = readOnly;
        m_lessonTextEdit->setReadOnly(readOnly);
        m_openTextFileButton->setEnabled(!readOnly);
        m_reformatLessonTextButton->setEnabled(!readOnly);
    }
}

KTextEdit* LessonTextEditor::textEdit() const
{
    return m_lessonTextEdit;
}

LessonTextHighlighter* LessonTextEditor::highlighter() const
{
    return m_highlighter;
}

void LessonTextEditor::onLessonTextChanged()
{
    const QString text = m_lessonTextEdit->toPlainText();
    const int length = text.length();

    m_characterCountLabel->setText(i18np("%1 Character", "%1 Characters", length));
}

void LessonTextEditor::openTextFile()
{
    const QString path(KFileDialog::getOpenFileName(KUrl("kfiledialog:///text"), QString("text/plain"), this));

    if (path.isNull())
        return;

    QFile textFile(path);

    if (!textFile.open(QIODevice::ReadOnly))
    {
        KMessageBox::error(this, i18n("The selected file could not be opened"));
        return;
    }

    const QString text = QString::fromLocal8Bit(textFile.readAll());

    m_lessonTextEdit->setPlainText(text);
}

void LessonTextEditor::reformatText()
{
    QTextCursor cursor(m_lessonTextEdit->textCursor());

    const bool doPartialReplace = cursor.hasSelection() && !cursor.hasComplexSelection();

    if (doPartialReplace)
    {
        const int begin = cursor.selectionStart();
        const int end = cursor.selectionEnd();
        cursor.setPosition(begin, QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
        cursor.setPosition(end, QTextCursor::KeepAnchor);
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    }

    QString text = doPartialReplace? cursor.selectedText(): m_lessonTextEdit->toPlainText();

    text = text.replace('\r', " ");
    text = text.replace('\n', " ");
    text = text.replace(QString::fromUtf8("\u2029"), " ");

    const QStringList tokens(text.split(' '));
    QStringList lines;
    QString currentLine("");

    foreach(const QString& token, tokens)
    {
        if (token.isEmpty())
            continue;

        if (currentLine.length() + token.length() + 1 > 60)
        {
            lines << currentLine;
            currentLine = token;
        }
        else
        {
            currentLine += currentLine.isEmpty()? token: ' ' + token;
        }
    }

    if (!currentLine.isEmpty())
    {
        lines.append(currentLine);
    }

    text = lines.join("\n");

    if (!doPartialReplace)
    {
        cursor.select(QTextCursor::Document);
    }

    cursor.removeSelectedText();
    cursor.insertText(text);
}

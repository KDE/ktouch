/*
 * SPDX-FileCopyrightText: 2013 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "lessontexteditor.h"

#include <QFileDialog>
#include <QTextCursor>
#include <QTextDocumentFragment>

#include <KLocalizedString>
#include <KMessageBox>

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
    connect(m_openTextFileButton, &QAbstractButton::clicked, this, &LessonTextEditor::openTextFile);
    connect(m_reformatLessonTextButton, &QAbstractButton::clicked, this, &LessonTextEditor::reformatText);
    connect(m_lessonTextEdit, &QTextEdit::textChanged, this, &LessonTextEditor::onLessonTextChanged);
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
    const QString path(QFileDialog::getOpenFileName(this, QString(), QString(), i18n("Text files (*.txt)")));

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

    text.replace('\r', QLatin1String(" "));
    text.replace('\n', QLatin1String(" "));
    text.replace(QString::fromUtf8("\u2029"), QLatin1String(" "));

    const QStringList tokens(text.split(' '));
    QStringList lines;
    QString currentLine(QLatin1String(""));

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

    text = lines.join(QLatin1Char('\n'));

    if (!doPartialReplace)
    {
        cursor.select(QTextCursor::Document);
    }

    cursor.removeSelectedText();
    cursor.insertText(text);
}

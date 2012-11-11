/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "traininglinecore.h"

#include <QKeyEvent>
#include <QTextBoundaryFinder>

#include "core/trainingstats.h"
#include "preferences.h"

TrainingLineCore::TrainingLineCore(QDeclarativeItem* parent) :
    QDeclarativeItem(parent),
    m_active(false),
    m_trainingStats(0),
    m_hintKey(-1),
    m_keyHintOccurrenceCount(0)
{
    setFlag(QDeclarativeItem::ItemAcceptsInputMethod);
}

bool TrainingLineCore::active() const
{
    return m_active;
}

void TrainingLineCore::setActive(bool active)
{
    if (active != m_active)
    {
        m_active = active;
        emit activeChanged();
    }
}

TrainingStats* TrainingLineCore::trainingStats() const
{
    return m_trainingStats;
}

void TrainingLineCore::setTrainingStats(TrainingStats* trainingStats)
{
    if (trainingStats != m_trainingStats)
    {
        m_trainingStats = trainingStats;
        emit trainingStatsChanged();
    }
}

QString TrainingLineCore::referenceLine() const
{
    return m_referenceLine;
}

void TrainingLineCore::setReferenceLine(const QString& referenceLine)
{
    if (referenceLine != m_referenceLine)
    {
        m_referenceLine = referenceLine;
        m_actualLine = "";
        emit referenceLineChanged();
        emit actualLineChanged();
    }
}

QString TrainingLineCore::actualLine() const
{
    return m_actualLine;
}

bool TrainingLineCore::isCorrect() const
{
    return m_actualLine == m_referenceLine.left(m_actualLine.length());
}

QString TrainingLineCore::nextCharacter() const
{
    const int actualLength = m_actualLine.length();

    if (actualLength < m_referenceLine.length())
    {
        return m_referenceLine.at(actualLength);
    }

    return QString();
}

int TrainingLineCore::hintKey() const
{
    return m_keyHintOccurrenceCount >= 3? m_hintKey: -1;
}

void TrainingLineCore::keyPressEvent(QKeyEvent* event)
{
    QDeclarativeItem::keyPressEvent(event);

    if (!m_active)
    {
        event->ignore();
        return;
    }

    if (m_referenceLine == m_actualLine)
    {
        if (Preferences::nextLineWithReturn())
        {
            if (event->key() == Qt::Key_Return)
            {
                reset();
                clearKeyHint();
                emit done();
                event->accept();
                return;
            }
            else
            {
                giveKeyHint(Qt::Key_Return);
            }
        }
        else if (Preferences::nextLineWithSpace())
        {
            if (event->key() == Qt::Key_Space)
            {
                reset();
                clearKeyHint();
                emit done();
                event->accept();
                return;
            }
            else
            {
                giveKeyHint(Qt::Key_Space);
            }
        }
    }

    bool unknown = false;

    if (event == QKeySequence::DeleteStartOfWord)
    {
        deleteStartOfWord();
    }
    else
    {
        if (event->modifiers() & Qt::ControlModifier)
        {
            switch (event->key())
            {
            case Qt::Key_Backspace:
                deleteStartOfWord();
                break;
            default:
                unknown = true;
            }
        }
        else
        {
            switch (event->key())
            {
            case Qt::Key_Backspace:
                backspace();
                break;
            default:
                unknown = true;
            }
        }
    }

    if (unknown)
    {
        const QString text = event->text();
        if (!text.isEmpty() && text.at(0).isPrint())
        {
            add(text);
            event->accept();
            return;
        }
    }

    if (unknown)
        event->ignore();
    else
        event->accept();
}

void TrainingLineCore::inputMethodEvent(QInputMethodEvent *event)
{
    if (!m_active)
    {
        event->ignore();
        return;
    }

    const QString commitString = event->commitString();

    if (!commitString.isEmpty())
    {
        add(commitString);
    }

    event->accept();
}

QVariant TrainingLineCore::inputMethodQuery(Qt::InputMethodQuery query) const
{
    switch (query)
    {
    case Qt::ImCursorPosition:
        return QVariant(m_actualLine.length());
    case Qt::ImSurroundingText:
        return QVariant(m_actualLine);
    case Qt::ImCurrentSelection:
        return QVariant("");
    case Qt::ImMaximumTextLength:
        return QVariant(m_referenceLine.length());
    case Qt::ImAnchorPosition:
        return QVariant(m_actualLine.length());
    default:
        return QVariant();
    }
}

void TrainingLineCore::add(const QString& text)
{
    const int maxLength = m_referenceLine.length();
    const int actualLength = m_actualLine.length();

    const QString newText = text.left(maxLength - actualLength);
    bool correct = isCorrect();

    for (int i = 0; i < newText.length(); i++)
    {
        const QString character(newText.at(i));
        const bool characterIsCorrect = character == m_referenceLine.at(actualLength + i);

        if (m_trainingStats)
        {
            m_trainingStats->logCharacter(character, characterIsCorrect? TrainingStats::CorrectCharacter: TrainingStats::IncorrectCharacter);
        }

        correct = correct && characterIsCorrect;

        if (correct)
        {
            clearKeyHint();
        }
        else
        {
            giveKeyHint(Qt::Key_Backspace);
        }
    }

    m_actualLine += text.left(maxLength - actualLength);
    emit actualLineChanged();
}

void TrainingLineCore::backspace()
{
    const int actualLength = m_actualLine.length();

    if (actualLength > 0)
    {
        m_actualLine = m_actualLine.left(actualLength - 1);
        emit actualLineChanged();

        if (isCorrect())
        {
            clearKeyHint();
        }
    }
}

void TrainingLineCore::deleteStartOfWord()
{
    const int actualLength = m_actualLine.length();

    if (actualLength > 0)
    {
        QTextBoundaryFinder finder(QTextBoundaryFinder::Word, m_actualLine);

        finder.setPosition(actualLength);
        finder.toPreviousBoundary();

        m_actualLine = m_actualLine.left(finder.position());
        emit actualLineChanged();
    }
}

void TrainingLineCore::reset()
{
    m_actualLine = "";
    emit actualLineChanged();
}

void TrainingLineCore::giveKeyHint(int key)
{
    if (key == m_hintKey)
    {
        m_keyHintOccurrenceCount++;
    }
    else
    {
        m_hintKey = key;
        m_keyHintOccurrenceCount = 1;
    }

    emit hintKeyChanged();
}

void TrainingLineCore::clearKeyHint()
{
    m_hintKey = -1;
    m_keyHintOccurrenceCount = 0;

    emit hintKeyChanged();
}

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


#include "keyboardlayouteditor.h"

#include <qdeclarative.h>

#include <QDeclarativeContext>
#include <QUndoStack>

#include <KStandardDirs>
#include <KMessageBox>
#include <kdeclarative.h>

#include "core/dataindex.h"
#include "core/dataaccess.h"
#include "core/keyboardlayout.h"
#include "core/abstractkey.h"
#include "core/key.h"
#include "core/keychar.h"

KeyboardLayoutEditor::KeyboardLayoutEditor(QWidget* parent):
    AbstractEditor(parent),
    m_dataIndexKeyboardLayout(0),
    m_keyboardLayout(new KeyboardLayout(this)),
    m_readOnly(false),
    m_selectedKey(0)
{
    setupUi(this);
    m_messageWidget->hide();

    KDeclarative kDeclarative;
    kDeclarative.setDeclarativeEngine(m_view->engine());
    kDeclarative.initialize();
    kDeclarative.setupBindings();

    m_view->rootContext()->setContextObject(this);
    m_view->setSource(QUrl::fromLocalFile(KGlobal::dirs()->findResource("appdata", "qml/KeyboardLayoutEditor.qml")));

    connect(m_view, SIGNAL(clicked()), SLOT(clearSelection()));
}

void KeyboardLayoutEditor::openKeyboardLayout(DataIndexKeyboardLayout* dataIndexKeyboardLayout)
{
    DataAccess dataAccess;

    m_dataIndexKeyboardLayout = dataIndexKeyboardLayout;

    const QString path = dataIndexKeyboardLayout->path();

    initUndoStack(path);
    setSelectedKey(0);

    if (!dataAccess.loadKeyboardLayout(path, m_keyboardLayout))
    {
        KMessageBox::error(this, i18n("Error while opening keyboard layout"));
    }

    if (dataIndexKeyboardLayout->source() == DataIndex::BuiltInResource)
    {
        setReadOnly(true);
        m_messageWidget->setMessageType(KMessageWidget::Information);
        m_messageWidget->setText(i18n("Built-in keyboard layouts can only be viewed."));
        m_messageWidget->setCloseButtonVisible(false);
        m_messageWidget->animatedShow();
    }
    else
    {
        setReadOnly(false);
        m_messageWidget->animatedHide();
    }
}

void KeyboardLayoutEditor::clearUndoStackForKeyboardLayout(DataIndexKeyboardLayout* dataIndexKeyboardLayout)
{
    clearUndoStack(dataIndexKeyboardLayout->path());
}

void KeyboardLayoutEditor::save()
{
    if (!m_keyboardLayout || !m_keyboardLayout->isValid())
        return;

    if (currentUndoStack()->isClean())
        return;

    DataAccess dataAccess;

    dataAccess.storeKeyboardLayout(m_dataIndexKeyboardLayout->path(), m_keyboardLayout);
    currentUndoStack()->setClean();
}

KeyboardLayout* KeyboardLayoutEditor::keyboardLayout() const
{
    return m_keyboardLayout;
}

bool KeyboardLayoutEditor::readOnly() const
{
    return m_readOnly;
}

void KeyboardLayoutEditor::setReadOnly(bool readOnly)
{
    if (readOnly != m_readOnly)
    {
        m_readOnly = readOnly;
        emit readOnlyChanged();
    }
}

AbstractKey* KeyboardLayoutEditor::selectedKey() const
{
    return m_selectedKey;
}

void KeyboardLayoutEditor::setSelectedKey(AbstractKey* key)
{
    if (key != m_selectedKey)
    {
        m_selectedKey = key;
        emit selectedKeyChanged();
    }
}

void KeyboardLayoutEditor::clearSelection()
{
    setSelectedKey(0);
}

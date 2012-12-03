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

#include <math.h>

#include <qdeclarative.h>

#include <QDeclarativeContext>
#include <QUndoStack>

#include <KStandardDirs>
#include <KMessageBox>
#include <KDebug>

#include "core/dataindex.h"
#include "core/dataaccess.h"
#include "core/keyboardlayout.h"
#include "core/abstractkey.h"
#include "core/key.h"
#include "core/keychar.h"
#include "core/userdataaccess.h"
#include "undocommands/keyboardlayoutcommands.h"
#include "application.h"

KeyboardLayoutEditor::KeyboardLayoutEditor(QWidget* parent):
    AbstractEditor(parent),
    m_dataIndexKeyboardLayout(0),
    m_keyboardLayout(new KeyboardLayout(this)),
    m_readOnly(false),
    m_selectedKey(0),
    m_zoomLevel(0)
{
    setupUi(this);
    m_messageWidget->hide();
    m_propertiesWidget->setKeyboardLayout(m_keyboardLayout);

    Application::setupDeclarativeBindings(m_view->engine());

    m_view->rootContext()->setContextObject(this);
    m_view->setSource(QUrl::fromLocalFile(KGlobal::dirs()->findResource("appdata", "qml/KeyboardLayoutEditor.qml")));

    connect(m_newKeyToolButton, SIGNAL(clicked()), SLOT(createNewKey()));
    connect(m_newSpecialKeyToolButton, SIGNAL(clicked()), SLOT(createNewSpecialKey()));
    connect(m_zoomSlider, SIGNAL(valueChanged(int)), SLOT(setZoomLevel(int)));
    connect(m_deleteKeyToolButton, SIGNAL(clicked(bool)), SLOT(deleteSelectedKey()));
    connect(m_view, SIGNAL(clicked()), SLOT(clearSelection()));
}

void KeyboardLayoutEditor::openKeyboardLayout(DataIndexKeyboardLayout* dataIndexKeyboardLayout)
{
    DataAccess dataAccess;

    m_dataIndexKeyboardLayout = dataIndexKeyboardLayout;

    if (currentUndoStack())
    {
        currentUndoStack()->disconnect(this, SLOT(validateSelection()));
    }

    initUndoStack(QString("keyboard-layout-%1").arg(dataIndexKeyboardLayout->id()));
    m_propertiesWidget->setUndoStack(currentUndoStack());
    setSelectedKey(0);
    connect(currentUndoStack(), SIGNAL(indexChanged(int)), SLOT(validateSelection()));

    m_keyboardLayout->setAssociatedDataIndexKeyboardLayout(m_dataIndexKeyboardLayout);

    if (!dataAccess.loadKeyboardLayout(dataIndexKeyboardLayout, m_keyboardLayout))
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

    UserDataAccess userDataAccess;

    userDataAccess.storeKeyboardLayout(m_keyboardLayout);
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
        m_newKeyToolButton->setEnabled(!readOnly);
        m_newSpecialKeyToolButton->setEnabled(!readOnly);
        m_deleteKeyToolButton->setEnabled(!readOnly && m_selectedKey != 0);
        m_propertiesWidget->setReadOnly(readOnly);
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

        m_deleteKeyToolButton->setEnabled(!m_readOnly && m_selectedKey != 0);
        m_propertiesWidget->setSelectedKey(m_keyboardLayout->keyIndex(key));
    }
}

int KeyboardLayoutEditor::zoomLevel() const
{
    return m_zoomLevel;
}

void KeyboardLayoutEditor::setZoomLevel(int zoomLevel)
{
    if (zoomLevel != m_zoomLevel)
    {
        m_zoomLevel = zoomLevel;
        emit zoomLevelChanged();
        const double zoomFactor = pow(2.0, zoomLevel / 2.0);
        m_zoomFactorLabel->setText(ki18n("%1%").subs(zoomFactor * 100, 0, 'f', 0).toString());
        m_zoomOutToolButton->setEnabled(zoomLevel > m_zoomSlider->minimum());
        m_zoomInToolButton->setEnabled(zoomLevel < m_zoomSlider->maximum());
    }
}

void KeyboardLayoutEditor::setKeyGeometry(int keyIndex, int top, int left, int width, int height)
{
    QRect rect(top, left, width, height);

    if (rect != keyboardLayout()->key(keyIndex)->rect())
    {
        QUndoCommand* command = new SetKeyGeometryCommand(keyboardLayout(), keyIndex, rect);
        currentUndoStack()->push(command);
    }
}

void KeyboardLayoutEditor::clearSelection()
{
    setSelectedKey(0);
}

void KeyboardLayoutEditor::validateSelection()
{
    if (m_keyboardLayout->keyIndex(m_selectedKey) == -1)
    {
        clearSelection();
    }
}

void KeyboardLayoutEditor::createNewKey()
{
    Key* key = new Key();
    key->setRect(QRect(0, 0, 80, 80));
    QUndoCommand* command = new AddKeyCommand(m_keyboardLayout, key);
    currentUndoStack()->push(command);
    setSelectedKey(key);
}

void KeyboardLayoutEditor::createNewSpecialKey()
{
    SpecialKey* key = new SpecialKey();
    key->setRect(QRect(0, 0, 130, 80));
    QUndoCommand* command = new AddKeyCommand(m_keyboardLayout, key);
    currentUndoStack()->push(command);
    setSelectedKey(key);
}

void KeyboardLayoutEditor::deleteSelectedKey()
{
    Q_ASSERT(m_selectedKey);

    const int keyIndex = m_keyboardLayout->keyIndex(m_selectedKey);
    QUndoCommand* command = new RemoveKeyCommand(m_keyboardLayout, keyIndex);

    setSelectedKey(0);
    currentUndoStack()->push(command);
}

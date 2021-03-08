/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */


#include "keyboardlayouteditor.h"

#include <math.h>

#include <QUndoStack>
#include <QStandardPaths>
#include <QQmlContext>

#include <KLocalizedString>
#include <KMessageBox>

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
    m_selectedKey(nullptr),
    m_zoomLevel(0)
{
    setupUi(this);
    m_messageWidget->hide();
    m_propertiesWidget->setKeyboardLayout(m_keyboardLayout);

    Application::setupDeclarativeBindings(m_view->engine());

    m_view->rootContext()->setContextProperty(QStringLiteral("keyboardLayoutEditor"), this);
    m_view->setSource(QUrl(QStringLiteral("qrc:/ktouch/qml/keyboard/KeyboardLayoutEditor.qml")));

    connect(m_newKeyToolButton, &QAbstractButton::clicked, this, &KeyboardLayoutEditor::createNewKey);
    connect(m_newSpecialKeyToolButton, &QAbstractButton::clicked, this, &KeyboardLayoutEditor::createNewSpecialKey);
    connect(m_zoomSlider, &QAbstractSlider::valueChanged, this, &KeyboardLayoutEditor::setZoomLevel);
    connect(m_deleteKeyToolButton, &QAbstractButton::clicked, this, &KeyboardLayoutEditor::deleteSelectedKey);
    connect(m_view, &KeyboardLayoutEditorView::clicked, this, &KeyboardLayoutEditor::clearSelection);

    connect(m_zoomOutToolButton, &QToolButton::clicked, [=](){
        m_zoomSlider->setValue(m_zoomSlider->value() - 1);
    });
    connect(m_zoomInToolButton, &QToolButton::clicked, [=](){
        m_zoomSlider->setValue(m_zoomSlider->value() + 1);
    });

}

KeyboardLayoutEditor::~KeyboardLayoutEditor()
{
    m_view->setSource(QUrl());
    delete m_view;
    m_view = nullptr;
    m_selectedKey = nullptr;
}

void KeyboardLayoutEditor::openKeyboardLayout(DataIndexKeyboardLayout* dataIndexKeyboardLayout)
{
    DataAccess dataAccess;

    m_dataIndexKeyboardLayout = dataIndexKeyboardLayout;

    if (currentUndoStack())
    {
        currentUndoStack()->disconnect(this, SLOT(validateSelection()));
    }

    initUndoStack(QStringLiteral("keyboard-layout-%1").arg(dataIndexKeyboardLayout->id()));
    m_propertiesWidget->setUndoStack(currentUndoStack());
    setSelectedKey(0);
    connect(currentUndoStack(), &QUndoStack::indexChanged, this, &KeyboardLayoutEditor::validateSelection);

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

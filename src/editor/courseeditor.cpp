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


#include "courseeditor.h"

#include <QUndoStack>
#include <QUndoGroup>

#include <KMessageBox>

#include "core/course.h"
#include "core/dataindex.h"
#include "core/dataaccess.h"

CourseEditor::CourseEditor(QWidget* parent):
    QWidget(parent),
    Ui::CourseEditor(),
    m_dataIndexCourse(0),
    m_course(new Course(this)),
    m_undoStacks(new QMap<QString,QUndoStack*>),
    m_currentUndoStack(0)
{
    setupUi(this);
    m_messageWidget->hide();

    connect(m_course, SIGNAL(titleChanged()), SLOT(updateTitle()));
    connect(m_course, SIGNAL(keyboardLayoutNameChanged()), SLOT(updateKeyboardLayoutName()));
    connect(m_course, SIGNAL(descriptionChanged()), SLOT(updateDescription()));

    connect(m_titleLineEdit, SIGNAL(textEdited(QString)), SLOT(setTitle(QString)));
    connect(m_keyboardLayoutComboBox, SIGNAL(activated(int)), SLOT(onKeyboardLayoutChosen()));
    connect(m_descriptionTextEdit, SIGNAL(textChanged()), SLOT(onDescriptionChanged()));
}

void CourseEditor::setResourceModel(ResourceModel* model)
{
    m_keyboardLayoutComboBox->setResourceModel(model);
}

void CourseEditor::setUndoGroup(QUndoGroup* undoGroup)
{
    m_undoGroup = undoGroup;
}

void CourseEditor::openCourse(DataIndexCourse* dataIndexCourse)
{
    DataAccess dataAccess;

    m_dataIndexCourse = dataIndexCourse;

    const QString coursePath = dataIndexCourse->path();

    if (m_undoStacks->contains(coursePath))
    {
        m_currentUndoStack = m_undoStacks->value(coursePath);
    }
    else
    {
        m_currentUndoStack = new QUndoStack(this);
        m_undoStacks->insert(coursePath, m_currentUndoStack);
        m_undoGroup->addStack(m_currentUndoStack);
    }

    m_undoGroup->setActiveStack(m_currentUndoStack);

    if (!dataAccess.loadCourse(coursePath, m_course))
    {
        KMessageBox::error(this, i18n("Error while opening course"));
    }

    if (dataIndexCourse->source() == DataIndex::BuiltInResource)
    {
        setIsReadOnly(true);
        m_messageWidget->setMessageType(KMessageWidget::Information);
        m_messageWidget->setText(i18n("Built-in courses can only be viewed."));
        m_messageWidget->setCloseButtonVisible(false);
        m_messageWidget->animatedShow();
    }
    else
    {
        setIsReadOnly(false);
        m_messageWidget->animatedHide();
    }
}

void CourseEditor::save()
{
    if (!m_course || !m_course->isValid())
        return;

    if (m_currentUndoStack->isClean())
        return;

    DataAccess dataAccess;

    dataAccess.storeCourse(m_dataIndexCourse->path(), m_course);
    m_currentUndoStack->setClean();
}

void CourseEditor::setTitle(const QString& newTitle)
{
    const QString oldTitle = m_course->title();
    m_course->setTitle(newTitle);
    QUndoCommand* command = new SetCourseTitleCommand(m_course, oldTitle);
    m_currentUndoStack->push(command);
}

void CourseEditor::setKeyboardLayoutName(const QString& newName)
{
    const QString oldName = m_course->keyboardLayoutName();
    m_course->setKeyboardLayoutName(newName);
    QUndoCommand* command = new SetCourseKeyboadLayoutNameCommand(m_course, oldName);
    m_currentUndoStack->push(command);
}

void CourseEditor::setDescription(const QString& newDescription)
{
    const QString oldDescription = m_course->description();
    m_course->setDescription(newDescription);
    QUndoCommand* command = new SetCourseDescriptionCommand(m_course, oldDescription);
    m_currentUndoStack->push(command);
}

void CourseEditor::updateTitle()
{
    const QString title = m_course->title();

    m_dataIndexCourse->setTitle(title);

    if (title != m_titleLineEdit->text())
    {
        m_titleLineEdit->setText(title);
    }
}

void CourseEditor::updateKeyboardLayoutName()
{
    const QString name = m_course->keyboardLayoutName();

    m_dataIndexCourse->setKeyboardLayoutName(name);

    for (int i = 0; m_keyboardLayoutComboBox->count(); i++)
    {
        if (m_keyboardLayoutComboBox->keyboardLayoutAt(i)->name() == name)
        {
            m_keyboardLayoutComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void CourseEditor::updateDescription()
{
    const QString description = m_course->description();

    m_dataIndexCourse->setDescription(description);

    if (description != m_descriptionTextEdit->toPlainText())
    {
        m_descriptionTextEdit->setPlainText(description);
    }
}

void CourseEditor::onKeyboardLayoutChosen()
{
    setKeyboardLayoutName(m_keyboardLayoutComboBox->selectedKeyboardLayout()->name());
}

void CourseEditor::onDescriptionChanged()
{
    const QString description = m_descriptionTextEdit->toPlainText();

    if (description != m_course->description())
    {
        setDescription(description);
    }
}

void CourseEditor::setIsReadOnly(bool readOnly)
{
    m_titleLineEdit->setReadOnly(readOnly);
    m_keyboardLayoutComboBox->setEnabled(!readOnly);
    m_descriptionTextEdit->setReadOnly(readOnly);
    m_addLessonButton->setEnabled(!readOnly);
    m_removeLessonButton->setEnabled(!readOnly);
    m_moveLessonUpButton->setEnabled(!readOnly);
    m_moveLessonDownButton->setEnabled(!readOnly);
    m_lessonTitleLineEdit->setReadOnly(readOnly);
    m_newCharactersLineEdit->setReadOnly(readOnly);
    m_lessonTextLineEdit->setReadOnly(readOnly);
}

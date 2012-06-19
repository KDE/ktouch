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

#include <QUuid>
#include <QUndoStack>
#include <QUndoGroup>

#include <KMessageBox>

#include "core/course.h"
#include "core/lesson.h"
#include "core/dataindex.h"
#include "core/dataaccess.h"
#include "lessonmodel.h"

CourseEditor::CourseEditor(QWidget* parent):
    QWidget(parent),
    Ui::CourseEditor(),
    m_dataIndexCourse(0),
    m_course(new Course(this)),
    m_currentLessonIndex(-1),
    m_currentLesson(0),
    m_lessonModel(new LessonModel(this)),
    m_readOnly(false),
    m_undoStacks(new QMap<QString,QUndoStack*>),
    m_currentUndoStack(0)
{
    setupUi(this);
    m_messageWidget->hide();
    m_lessonModel->setCourse(m_course);
    m_lessonView->setModel(m_lessonModel);

    connect(m_course, SIGNAL(titleChanged()), SLOT(updateTitle()));
    connect(m_course, SIGNAL(keyboardLayoutNameChanged()), SLOT(updateKeyboardLayoutName()));
    connect(m_course, SIGNAL(descriptionChanged()), SLOT(updateDescription()));

    connect(m_titleLineEdit, SIGNAL(textEdited(QString)), SLOT(setTitle(QString)));
    connect(m_keyboardLayoutComboBox, SIGNAL(activated(int)), SLOT(onKeyboardLayoutChosen()));
    connect(m_descriptionTextEdit, SIGNAL(textChanged()), SLOT(onDescriptionChanged()));

    connect(m_lessonModel, SIGNAL(lessonChanged(int)), SLOT(selectLesson(int)));
    connect(m_lessonView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(onLessonSelected()));

    connect(m_addLessonButton, SIGNAL(clicked(bool)), SLOT(addLesson()));
    connect(m_removeLessonButton, SIGNAL(clicked(bool)), SLOT(removeLesson()));
    connect(m_moveLessonUpButton, SIGNAL(clicked(bool)), SLOT(moveLessonUp()));
    connect(m_moveLessonDownButton, SIGNAL(clicked(bool)), SLOT(moveLessonDown()));

    connect(m_lessonTitleLineEdit, SIGNAL(textEdited(QString)), SLOT(setLessonTitle(QString)));
    connect(m_newCharactersLineEdit, SIGNAL(textEdited(QString)), SLOT(setLessonNewCharacters(QString)));
    connect(m_lessonTextEdit, SIGNAL(textChanged()), SLOT(onLessonTextChanged()));
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

    m_currentLessonIndex = -1;
    m_currentLesson = 0;

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

    if (m_course->lessonCount() > 0)
    {
        selectLesson(0);
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

void CourseEditor::addLesson()
{
    const int newIndex = m_currentLessonIndex + 1;
    const QString id = QUuid::createUuid();
    QUndoCommand* command = new AddLessonCommand(m_course, newIndex, id);

    m_currentUndoStack->push(command);
    selectLesson(newIndex);
}

void CourseEditor::removeLesson()
{
    Q_ASSERT(m_currentLessonIndex != -1);

    const int index = m_currentLessonIndex;
    QUndoCommand* command = new RemoveLessonCommand(m_course, index);

    m_lessonView->selectionModel()->clear();
    m_currentUndoStack->push(command);
    selectLesson(qMin(index, m_course->lessonCount() - 1));
}

void CourseEditor::moveLessonUp()
{
    Q_ASSERT(m_currentLessonIndex > 0);

    const int oldIndex = m_currentLessonIndex;
    const int newIndex = m_currentLessonIndex - 1;
    QUndoCommand* command = new MoveLessonCommand(m_course, oldIndex, newIndex);

    m_lessonView->selectionModel()->clear();
    m_currentUndoStack->push(command);
    selectLesson(newIndex);
}

void CourseEditor::moveLessonDown()
{
    Q_ASSERT(m_currentLessonIndex != -1 && m_currentLessonIndex < m_course->lessonCount() - 1);

    const int oldIndex = m_currentLessonIndex;
    const int newIndex = m_currentLessonIndex + 1;
    QUndoCommand* command = new MoveLessonCommand(m_course, oldIndex, newIndex);

    m_lessonView->selectionModel()->clear();
    m_currentUndoStack->push(command);
    selectLesson(newIndex);
}

void CourseEditor::setLessonTitle(const QString& newTitle)
{
    Q_ASSERT(m_currentLesson);
    const QString oldTitle = m_currentLesson->title();
    m_currentLesson->setTitle(newTitle);
    QUndoCommand* command = new SetLessonTitleCommand(m_course, m_currentLessonIndex, oldTitle);
    m_currentUndoStack->push(command);
}

void CourseEditor::setLessonNewCharacters(const QString& newCharacters)
{
    Q_ASSERT(m_currentLesson);
    const QString oldNewCharacters = m_currentLesson->newCharacters();
    m_currentLesson->setNewCharacters(newCharacters);
    QUndoCommand* command = new SetLessonNewCharactersCommand(m_course, m_currentLessonIndex, oldNewCharacters);
    m_currentUndoStack->push(command);
}

void CourseEditor::setLessonText(const QString& newText)
{
    Q_ASSERT(m_currentLesson);
    const QString oldText = m_currentLesson->text();
    m_currentLesson->setText(newText);
    QUndoCommand* command = new SetLessonTextCommand(m_course, m_currentLessonIndex, oldText);
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

void CourseEditor::updateLessonTitle()
{
    Q_ASSERT(m_currentLesson);

    const QString title = m_currentLesson->title();

    if (title != m_lessonTitleLineEdit->text())
    {
        m_lessonTitleLineEdit->setText(title);
    }
}

void CourseEditor::updateLessonNewCharacters()
{
    Q_ASSERT(m_currentLesson);

    const QString newCharacters = m_currentLesson->newCharacters();

    if (newCharacters != m_newCharactersLineEdit->text())
    {
        m_newCharactersLineEdit->setText(newCharacters);
    }
}

void CourseEditor::updateLessonText()
{
    Q_ASSERT(m_currentLesson);

    const QString text = m_currentLesson->text();

    if (text != m_lessonTextEdit->toPlainText())
    {
        m_lessonTextEdit->setText(text);
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

void CourseEditor::onLessonTextChanged()
{
    if (!m_currentLesson)
        return;

    const QString text = m_lessonTextEdit->toPlainText();

    if (text != m_currentLesson->text())
    {
        setLessonText(text);
    }
}

void CourseEditor::selectLesson(int index)
{
    if (index == -1)
        return;

    m_lessonView->selectionModel()->select(m_lessonModel->index(index), QItemSelectionModel::ClearAndSelect);
}

void CourseEditor::onLessonSelected()
{
    if (m_currentLesson)
    {
        m_currentLesson->disconnect(this);
    }

    if (m_lessonView->selectionModel()->hasSelection())
    {
        m_currentLessonIndex = m_lessonView->selectionModel()->selectedIndexes().first().row();
        m_currentLesson = m_course->lesson(m_currentLessonIndex);

        m_lessonTitleLineEdit->setEnabled(true);
        m_lessonTitleLineEdit->setText(m_currentLesson->title());
        m_newCharactersLineEdit->setEnabled(true);
        m_newCharactersLineEdit->setText(m_currentLesson->newCharacters());
        m_lessonTextEdit->setEnabled(true);
        m_lessonTextEdit->setText(m_currentLesson->text());

        m_removeLessonButton->setEnabled(!m_readOnly);
        m_moveLessonUpButton->setEnabled(!m_readOnly && m_currentLessonIndex > 0);
        m_moveLessonDownButton->setEnabled(!m_readOnly && m_currentLessonIndex < m_course->lessonCount() - 1);

        connect(m_currentLesson, SIGNAL(titleChanged()), SLOT(updateLessonTitle()));
        connect(m_currentLesson, SIGNAL(newCharactersChanged()), SLOT(updateLessonNewCharacters()));
        connect(m_currentLesson, SIGNAL(textChanged()), SLOT(updateLessonText()));
    }
    else
    {
        m_currentLessonIndex = -1;
        m_currentLesson = 0;

        m_lessonTitleLineEdit->setEnabled(false);
        m_lessonTitleLineEdit->clear();
        m_newCharactersLineEdit->setEnabled(false);
        m_newCharactersLineEdit->clear();
        m_lessonTextEdit->setEnabled(false);
        m_lessonTextEdit->clear();

        m_removeLessonButton->setEnabled(false);
        m_moveLessonUpButton->setEnabled(false);
        m_moveLessonDownButton->setEnabled(false);
    }
}

void CourseEditor::setIsReadOnly(bool readOnly)
{
    m_readOnly = readOnly;
    m_titleLineEdit->setReadOnly(readOnly);
    m_keyboardLayoutComboBox->setEnabled(!readOnly);
    m_descriptionTextEdit->setReadOnly(readOnly);
    m_addLessonButton->setEnabled(!readOnly);
    m_lessonTitleLineEdit->setReadOnly(readOnly);
    m_newCharactersLineEdit->setReadOnly(readOnly);
    m_lessonTextEdit->setReadOnly(readOnly);
}

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

#include <KMessageBox>

#include "core/course.h"
#include "core/lesson.h"
#include "core/dataindex.h"
#include "core/dataaccess.h"
#include "core/userdataaccess.h"
#include "models/lessonmodel.h"
#include "editor/lessontexthighlighter.h"
#include "editor/lessontexteditor.h"

CourseEditor::CourseEditor(QWidget* parent):
    AbstractEditor(parent),
    Ui::CourseEditor(),
    m_dataIndexCourse(0),
    m_course(new Course(this)),
    m_currentLessonIndex(-1),
    m_currentLesson(0),
    m_lessonModel(new LessonModel(this)),
    m_readOnly(false)
{
    setupUi(this);
    m_messageWidget->hide();
    m_lessonsSplitter->setStretchFactor(1, 3);
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
    connect(m_lessonTextEditor->textEdit(), SIGNAL(textChanged()), SLOT(onLessonTextChanged()));
}

void CourseEditor::setResourceModel(ResourceModel* model)
{
    m_keyboardLayoutComboBox->setResourceModel(model);
}

void CourseEditor::openCourse(DataIndexCourse* dataIndexCourse)
{
    DataAccess dataAccess;

    m_dataIndexCourse = dataIndexCourse;

    initUndoStack(QString("course-%1").arg(dataIndexCourse->id()));

    m_currentLessonIndex = -1;
    m_currentLesson = 0;

    m_course->setAssociatedDataIndexCourse(m_dataIndexCourse);

    if (!dataAccess.loadCourse(dataIndexCourse, m_course))
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

void CourseEditor::clearUndoStackForCourse(DataIndexCourse* course)
{
    clearUndoStack(course->path());
}

void CourseEditor::save()
{
    if (!m_course || !m_course->isValid())
        return;

    if (currentUndoStack()->isClean())
        return;

    UserDataAccess userDataAccess;

    userDataAccess.storeCourse(m_course);
    currentUndoStack()->setClean();
}

void CourseEditor::setTitle(const QString& newTitle)
{
    const QString oldTitle = m_course->title();
    m_course->setTitle(newTitle);
    QUndoCommand* command = new SetCourseTitleCommand(m_course, oldTitle);
    currentUndoStack()->push(command);
}

void CourseEditor::setKeyboardLayoutName(const QString& newName)
{
    const QString oldName = m_course->keyboardLayoutName();
    m_course->setKeyboardLayoutName(newName);
    QUndoCommand* command = new SetCourseKeyboadLayoutNameCommand(m_course, oldName);
    currentUndoStack()->push(command);
}

void CourseEditor::setDescription(const QString& newDescription)
{
    const QString oldDescription = m_course->description();
    m_course->setDescription(newDescription);
    QUndoCommand* command = new SetCourseDescriptionCommand(m_course, oldDescription);
    currentUndoStack()->push(command);
}

void CourseEditor::addLesson()
{
    const int newIndex = m_currentLessonIndex + 1;
    const QString id = QUuid::createUuid();
    QUndoCommand* command = new AddLessonCommand(m_course, newIndex, id);

    currentUndoStack()->push(command);
    selectLesson(newIndex);
}

void CourseEditor::removeLesson()
{
    Q_ASSERT(m_currentLessonIndex != -1);

    const int index = m_currentLessonIndex;
    QUndoCommand* command = new RemoveLessonCommand(m_course, index);

    m_lessonView->selectionModel()->clear();
    currentUndoStack()->push(command);
    selectLesson(qMin(index, m_course->lessonCount() - 1));
}

void CourseEditor::moveLessonUp()
{
    Q_ASSERT(m_currentLessonIndex > 0);

    const int oldIndex = m_currentLessonIndex;
    const int newIndex = m_currentLessonIndex - 1;
    QUndoCommand* command = new MoveLessonCommand(m_course, oldIndex, newIndex);

    m_lessonView->selectionModel()->clear();
    currentUndoStack()->push(command);
    selectLesson(newIndex);
}

void CourseEditor::moveLessonDown()
{
    Q_ASSERT(m_currentLessonIndex != -1 && m_currentLessonIndex < m_course->lessonCount() - 1);

    const int oldIndex = m_currentLessonIndex;
    const int newIndex = m_currentLessonIndex + 1;
    QUndoCommand* command = new MoveLessonCommand(m_course, oldIndex, newIndex);

    m_lessonView->selectionModel()->clear();
    currentUndoStack()->push(command);
    selectLesson(newIndex);
}

void CourseEditor::setLessonTitle(const QString& newTitle)
{
    Q_ASSERT(m_currentLesson);
    const QString oldTitle = m_currentLesson->title();
    m_currentLesson->setTitle(newTitle);
    QUndoCommand* command = new SetLessonTitleCommand(m_course, m_currentLessonIndex, oldTitle);
    currentUndoStack()->push(command);
}

void CourseEditor::setLessonNewCharacters(const QString& newCharacters)
{
    Q_ASSERT(m_currentLesson);
    const QString oldNewCharacters = m_currentLesson->newCharacters();
    m_currentLesson->setNewCharacters(newCharacters);
    QUndoCommand* command = new SetLessonNewCharactersCommand(m_course, m_currentLessonIndex, oldNewCharacters);
    currentUndoStack()->push(command);
}

void CourseEditor::setLessonText(const QString& newText)
{
    Q_ASSERT(m_currentLesson);
    const QString oldText = m_currentLesson->text();
    m_currentLesson->setText(newText);
    QUndoCommand* command = new SetLessonTextCommand(m_course, m_currentLessonIndex, oldText);
    currentUndoStack()->push(command);
}

void CourseEditor::updateTitle()
{
    const QString title = m_course->title();

    if (title != m_titleLineEdit->text())
    {
        m_titleLineEdit->setText(title);
    }
}

void CourseEditor::updateKeyboardLayoutName()
{
    const QString name = m_course->keyboardLayoutName();

    for (int i = 0; i < m_keyboardLayoutComboBox->count(); i++)
    {
        if (m_keyboardLayoutComboBox->keyboardLayoutAt(i)->name() == name)
        {
            m_keyboardLayoutComboBox->setCurrentIndex(i);
            return;
        }
    }

    m_keyboardLayoutComboBox->setCurrentIndex(-1);
}

void CourseEditor::updateDescription()
{
    const QString description = m_course->description();

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

    if (text != m_lessonTextEditor->textEdit()->toPlainText())
    {
        m_lessonTextEditor->textEdit()->setText(text);
    }
}

void CourseEditor::updateLessonCharacters()
{
    Q_ASSERT(m_currentLesson);
    m_lessonTextEditor->highlighter()->setAllowedCharacters(m_currentLesson->characters());
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

    const QString text = m_lessonTextEditor->textEdit()->toPlainText();

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
        m_lessonTextEditor->setEnabled(true);
        m_lessonTextEditor->textEdit()->setPlainText(m_currentLesson->text());

        m_lessonTextEditor->highlighter()->setAllowedCharacters(m_currentLesson->characters());

        m_removeLessonButton->setEnabled(!m_readOnly);
        m_moveLessonUpButton->setEnabled(!m_readOnly && m_currentLessonIndex > 0);
        m_moveLessonDownButton->setEnabled(!m_readOnly && m_currentLessonIndex < m_course->lessonCount() - 1);

        connect(m_currentLesson, SIGNAL(titleChanged()), SLOT(updateLessonTitle()));
        connect(m_currentLesson, SIGNAL(newCharactersChanged()), SLOT(updateLessonNewCharacters()));
        connect(m_currentLesson, SIGNAL(textChanged()), SLOT(updateLessonText()));
        connect(m_currentLesson, SIGNAL(charactersChanged()), SLOT(updateLessonCharacters()));
    }
    else
    {
        m_currentLessonIndex = -1;
        m_currentLesson = 0;

        m_lessonTitleLineEdit->setEnabled(false);
        m_lessonTitleLineEdit->clear();
        m_newCharactersLineEdit->setEnabled(false);
        m_newCharactersLineEdit->clear();
        m_lessonTextEditor->setEnabled(false);
        m_lessonTextEditor->textEdit()->clear();

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
    m_lessonTextEditor->setReadOnly(readOnly);
}

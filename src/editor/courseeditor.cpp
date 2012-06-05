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

#include "KMessageBox"

#include "core/course.h"
#include "core/dataindex.h"
#include "core/dataaccess.h"

CourseEditor::CourseEditor(QWidget* parent):
    QWidget(parent),
    Ui::CourseEditor(),
    m_dataIndexCourse(0),
    m_course(new Course(this))
{
    setupUi(this);
    m_messageWidget->hide();
}

void CourseEditor::setResourceModel(ResourceModel* model)
{
    m_keyboardLayoutComboBox->setResourceModel(model);
}

void CourseEditor::openCourse(DataIndexCourse* dataIndexCourse)
{
    DataAccess dataAccess;

    m_dataIndexCourse = dataIndexCourse;

    if (!dataAccess.loadCourse(dataIndexCourse->path(), m_course))
    {
        KMessageBox::error(this, i18n("Error while opening course"));
    }

    m_titleLineEdit->setText(m_course->title());
    m_descriptionTextEdit->setPlainText(m_course->description());

    for (int i = 0; m_keyboardLayoutComboBox->count(); i++)
    {
        if (m_keyboardLayoutComboBox->keyboardLayoutAt(i)->name() == m_course->keyboardLayoutName())
        {
            m_keyboardLayoutComboBox->setCurrentIndex(i);
            break;
        }
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

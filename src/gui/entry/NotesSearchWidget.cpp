/*
 *  Copyright (C) 2025 KeePassXC Team <team@keepassxc.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "NotesSearchWidget.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>

#include "gui/Icons.h"

NotesSearchWidget::NotesSearchWidget(QWidget* parent)
    : QWidget(parent)
{
    setObjectName("NotesSearchWidget");
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(4);

    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setObjectName("searchEdit");
    m_searchEdit->setPlaceholderText(tr("Find in notes…"));
    m_searchEdit->setClearButtonEnabled(true);

    m_prevButton = new QToolButton(this);
    m_prevButton->setIcon(icons()->icon("move-up"));
    m_prevButton->setToolTip(tr("Find previous"));

    m_nextButton = new QToolButton(this);
    m_nextButton->setIcon(icons()->icon("move-down"));
    m_nextButton->setToolTip(tr("Find next"));

    m_closeButton = new QToolButton(this);
    m_closeButton->setIcon(icons()->icon("dialog-close"));
    m_closeButton->setToolTip(tr("Close search"));

    m_caseSensitiveCheck = new QCheckBox(tr("Case sensitive"), this);

    layout->addWidget(m_searchEdit, 1);
    layout->addWidget(m_prevButton);
    layout->addWidget(m_nextButton);
    layout->addWidget(m_caseSensitiveCheck);
    layout->addWidget(m_closeButton);

    connect(m_searchEdit, &QLineEdit::textChanged, this, &NotesSearchWidget::searchTextChanged);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &NotesSearchWidget::findNext);
    connect(m_prevButton, &QToolButton::clicked, this, &NotesSearchWidget::findPrevious);
    connect(m_nextButton, &QToolButton::clicked, this, &NotesSearchWidget::findNext);
    connect(m_closeButton, &QToolButton::clicked, this, &NotesSearchWidget::closeRequested);
    connect(m_caseSensitiveCheck, &QCheckBox::toggled, this, &NotesSearchWidget::caseSensitiveChanged);

    m_searchEdit->installEventFilter(this);
}

bool NotesSearchWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == m_searchEdit && event->type() == QEvent::KeyPress) {
        auto* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Escape) {
            emit closeRequested();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

QString NotesSearchWidget::searchText() const
{
    return m_searchEdit->text();
}

bool NotesSearchWidget::caseSensitive() const
{
    return m_caseSensitiveCheck->isChecked();
}

void NotesSearchWidget::setSearchText(const QString& text)
{
    m_searchEdit->setText(text);
    m_searchEdit->selectAll();
}

void NotesSearchWidget::setMatchCount(int current, int total)
{
    Q_UNUSED(current)
    Q_UNUSED(total)
}

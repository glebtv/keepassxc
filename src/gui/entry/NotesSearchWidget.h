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

#ifndef KEEPASSX_NOTESSEARCHWIDGET_H
#define KEEPASSX_NOTESSEARCHWIDGET_H

#include <QWidget>

class QCheckBox;
class QLineEdit;
class QToolButton;

class NotesSearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NotesSearchWidget(QWidget* parent = nullptr);

    QString searchText() const;
    bool caseSensitive() const;
    void setSearchText(const QString& text);
    void setMatchCount(int current, int total);

signals:
    void searchTextChanged(const QString& text);
    void findNext();
    void findPrevious();
    void caseSensitiveChanged(bool sensitive);
    void closeRequested();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    QLineEdit* m_searchEdit;
    QToolButton* m_prevButton;
    QToolButton* m_nextButton;
    QToolButton* m_closeButton;
    QCheckBox* m_caseSensitiveCheck;
};

#endif // KEEPASSX_NOTESSEARCHWIDGET_H

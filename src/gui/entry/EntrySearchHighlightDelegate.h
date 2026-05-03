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

#ifndef KEEPASSX_ENTRY_SEARCH_HIGHLIGHT_DELEGATE_H
#define KEEPASSX_ENTRY_SEARCH_HIGHLIGHT_DELEGATE_H

#include <QRegularExpression>
#include <QStyledItemDelegate>

class EntrySearchHighlightDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit EntrySearchHighlightDelegate(QObject* parent = nullptr);

    void setSearchTerms(const QList<QRegularExpression>& terms);
    QList<QRegularExpression> searchTerms() const;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QList<QRegularExpression> m_searchTerms;
};

#endif // KEEPASSX_ENTRY_SEARCH_HIGHLIGHT_DELEGATE_H

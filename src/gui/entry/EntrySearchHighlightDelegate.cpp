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

#include "EntrySearchHighlightDelegate.h"

#include <QApplication>
#include <QPainter>
#include <QStyle>

EntrySearchHighlightDelegate::EntrySearchHighlightDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

void EntrySearchHighlightDelegate::setSearchTerms(const QList<QRegularExpression>& terms)
{
    m_searchTerms = terms;
}

QList<QRegularExpression> EntrySearchHighlightDelegate::searchTerms() const
{
    return m_searchTerms;
}

void EntrySearchHighlightDelegate::paint(QPainter* painter,
                                         const QStyleOptionViewItem& option,
                                         const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    if (m_searchTerms.isEmpty() || opt.text.isEmpty()) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    // Find all match regions in the original text
    QList<QPair<int, int>> matches;
    for (const auto& regex : m_searchTerms) {
        if (regex.pattern().isEmpty()) {
            continue;
        }
        auto it = regex.globalMatch(opt.text);
        while (it.hasNext()) {
            auto match = it.next();
            matches.append({match.capturedStart(), match.capturedEnd()});
        }
    }

    if (matches.isEmpty()) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    // Sort and merge overlapping matches
    std::sort(matches.begin(), matches.end());
    QList<QPair<int, int>> merged;
    for (const auto& m : matches) {
        if (merged.isEmpty() || m.first > merged.last().second) {
            merged.append(m);
        } else {
            merged.last().second = qMax(merged.last().second, m.second);
        }
    }

    QStyle* style = opt.widget ? opt.widget->style() : QApplication::style();

    // Draw background (including selection highlight)
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);

    // Draw icon
    QRect iconRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &opt, opt.widget);
    if (!opt.icon.isNull()) {
        opt.icon.paint(painter, iconRect, opt.decorationAlignment, QIcon::Normal, QIcon::Off);
    }

    // Get text drawing rect
    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &opt, opt.widget);

    QFontMetrics fm(opt.font);
    QColor textColor = (opt.state & QStyle::State_Selected)
                           ? opt.palette.color(QPalette::HighlightedText)
                           : opt.palette.color(QPalette::Text);
    QColor highlightBg(255, 235, 59); // #FFEB3B

    // Elide text if it doesn't fit
    QString fullText = opt.text;
    QString elidedText = fm.elidedText(fullText, Qt::ElideRight, textRect.width());
    bool wasElided = (elidedText.length() < fullText.length());

    // Calculate horizontal start position based on alignment
    int totalWidth = fm.horizontalAdvance(elidedText);
    int x = textRect.left();
    if (opt.displayAlignment & Qt::AlignRight) {
        x = textRect.right() - totalWidth;
    } else if (opt.displayAlignment & Qt::AlignHCenter) {
        x = textRect.left() + (textRect.width() - totalWidth) / 2;
    }

    // Vertical center baseline
    int baseline = textRect.top() + (textRect.height() - fm.height()) / 2 + fm.ascent();

    painter->setFont(opt.font);
    painter->setPen(textColor);

    // Draw text segments
    int pos = 0;
    for (const auto& m : merged) {
        // Non-highlighted segment before match
        if (pos < m.first && pos < elidedText.length()) {
            int segEnd = qMin(m.first, elidedText.length());
            QString seg = elidedText.mid(pos, segEnd - pos);
            int segWidth = fm.horizontalAdvance(seg);
            painter->drawText(x, baseline, seg);
            x += segWidth;
            pos = segEnd;
        }

        if (pos >= elidedText.length()) {
            break;
        }

        // Highlighted segment
        int matchEnd = qMin(m.second, elidedText.length());
        if (pos < matchEnd) {
            QString seg = elidedText.mid(pos, matchEnd - pos);
            int segWidth = fm.horizontalAdvance(seg);

            // Draw highlight background
            QRect hlRect(x, baseline - fm.ascent(), segWidth, fm.height());
            painter->fillRect(hlRect, highlightBg);

            // Draw text
            painter->drawText(x, baseline, seg);
            x += segWidth;
            pos = matchEnd;
        }

        if (pos >= elidedText.length()) {
            break;
        }
    }

    // Remaining non-highlighted text
    if (pos < elidedText.length()) {
        QString seg = elidedText.mid(pos);
        int segWidth = fm.horizontalAdvance(seg);
        painter->drawText(x, baseline, seg);
        x += segWidth;
    }

    // Draw focus rect if needed
    if (opt.state & QStyle::State_HasFocus) {
        QStyleOptionFocusRect focusOpt;
        focusOpt.initFrom(opt.widget);
        focusOpt.rect = opt.rect;
        focusOpt.backgroundColor = opt.palette.color(QPalette::Background);
        style->drawPrimitive(QStyle::PE_FrameFocusRect, &focusOpt, painter, opt.widget);
    }
}

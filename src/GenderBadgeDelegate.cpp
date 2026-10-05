#include "GenderBadgeDelegate.h"

#include <QPainter>

GenderBadgeDelegate::GenderBadgeDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

void GenderBadgeDelegate::paint(QPainter *painter,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) const {
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // Paint selection background, then the badge on top.
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, QColor("#26406E"));

    const QString gender = index.data(Qt::DisplayRole).toString();
    const bool isMale = (gender == "Male");
    const QColor bg = isMale ? QColor(59, 130, 246, 38) : QColor(139, 92, 246, 38);
    const QColor border = isMale ? QColor("#3B82F6") : QColor("#8B5CF6");
    const QColor text = isMale ? QColor("#60A5FA") : QColor("#A78BFA");

    // Centered pill sized to the text.
    QFont font = option.font;
    font.setBold(true);
    font.setPointSize(qMax(8, font.pointSize() - 2));
    painter->setFont(font);
    const QFontMetrics fm(font);
    const int textWidth = fm.horizontalAdvance(gender);
    const int pillW = textWidth + 28;
    const int pillH = qMin(option.rect.height() - 12, 26);
    const QRect pill(option.rect.center().x() - pillW / 2,
                     option.rect.center().y() - pillH / 2, pillW, pillH);

    painter->setBrush(bg);
    painter->setPen(QPen(border));
    painter->drawRoundedRect(pill, pillH / 2, pillH / 2);
    painter->setPen(text);
    painter->drawText(pill, Qt::AlignCenter, gender);
    painter->restore();
}

QSize GenderBadgeDelegate::sizeHint(const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const {
    QSize base = QStyledItemDelegate::sizeHint(option, index);
    base.setHeight(qMax(base.height(), 44));
    return base;
}

#pragma once

#include <QStyledItemDelegate>

// Paints the Gender cell as a centered colored pill badge.
class GenderBadgeDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit GenderBadgeDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};

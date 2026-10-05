#pragma once

#include <QSortFilterProxyModel>

#include "Student.h"

// Filters the student model by free-text search (ID + name) and gender.
// Sorting is inherited from QSortFilterProxyModel.
class StudentProxy : public QSortFilterProxyModel {
    Q_OBJECT
public:
    enum GenderFilter { AllGenders = 0, MalesOnly, FemalesOnly };

    explicit StudentProxy(QObject *parent = nullptr);

    void setSearchText(const QString &text);
    void setGenderFilter(GenderFilter filter);

protected:
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent) const override;

private:
    QString m_searchText;
    GenderFilter m_genderFilter = AllGenders;
};

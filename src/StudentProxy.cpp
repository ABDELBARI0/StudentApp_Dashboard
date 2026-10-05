#include "StudentProxy.h"

#include "StudentModel.h"

StudentProxy::StudentProxy(QObject *parent)
    : QSortFilterProxyModel(parent) {
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setSortCaseSensitivity(Qt::CaseInsensitive);
}

void StudentProxy::setSearchText(const QString &text) {
    if (m_searchText == text)
        return;
    m_searchText = text;
    invalidateFilter();
}

void StudentProxy::setGenderFilter(GenderFilter filter) {
    if (m_genderFilter == filter)
        return;
    m_genderFilter = filter;
    invalidateFilter();
}

bool StudentProxy::filterAcceptsRow(int sourceRow,
                                    const QModelIndex &sourceParent) const {
    const QAbstractItemModel *model = sourceModel();
    if (!model)
        return false;

    // Gender filter against the Gender column.
    if (m_genderFilter != AllGenders) {
        const QString gender =
            model->index(sourceRow, StudentModel::ColGender, sourceParent)
                .data(Qt::DisplayRole).toString();
        const bool isMale = (gender == "Male");
        if (m_genderFilter == MalesOnly && !isMale)
            return false;
        if (m_genderFilter == FemalesOnly && isMale)
            return false;
    }

    // Free-text search across ID and full name.
    if (!m_searchText.isEmpty()) {
        const QString id =
            model->index(sourceRow, StudentModel::ColId, sourceParent)
                .data(Qt::DisplayRole).toString();
        const QString name =
            model->index(sourceRow, StudentModel::ColName, sourceParent)
                .data(Qt::DisplayRole).toString();
        if (!id.contains(m_searchText, Qt::CaseInsensitive) &&
            !name.contains(m_searchText, Qt::CaseInsensitive))
            return false;
    }
    return true;
}

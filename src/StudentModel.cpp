#include "StudentModel.h"

StudentModel::StudentModel(QObject *parent)
    : QAbstractTableModel(parent), m_students(Student::mockStudents()) {}

int StudentModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : m_students.size();
}

int StudentModel::columnCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : ColCount;
}

QVariant StudentModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_students.size())
        return {};
    const Student &s = m_students.at(index.row());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case ColId: return s.id;
        case ColName: return s.fullName;
        case ColAge: return s.age;  // int: sorts numerically via the proxy
        case ColGender: return s.genderText();
        default: return {};
        }
    }
    if (role == Qt::TextAlignmentRole)
        return index.column() == ColAge
            ? QVariant(Qt::AlignCenter | Qt::AlignVCenter)
            : QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    return {};
}

QVariant StudentModel::headerData(int section, Qt::Orientation orientation,
                                 int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return {};
    switch (section) {
    case ColId: return "ID";
    case ColName: return "FULL NAME";
    case ColAge: return "AGE";
    case ColGender: return "GENDER";
    default: return {};
    }
}

void StudentModel::addStudent(const Student &student) {
    beginInsertRows(QModelIndex(), m_students.size(), m_students.size());
    m_students.append(student);
    endInsertRows();
}

void StudentModel::updateStudent(int row, const Student &student) {
    if (row < 0 || row >= m_students.size())
        return;
    m_students[row] = student;
    const QModelIndex top = index(row, 0);
    const QModelIndex bottom = index(row, ColCount - 1);
    emit dataChanged(top, bottom, {Qt::DisplayRole});
}

void StudentModel::removeStudent(int row) {
    if (row < 0 || row >= m_students.size())
        return;
    beginRemoveRows(QModelIndex(), row, row);
    m_students.removeAt(row);
    endRemoveRows();
}

Student StudentModel::studentAt(int row) const {
    return (row >= 0 && row < m_students.size()) ? m_students.at(row)
                                                 : Student{};
}

QString StudentModel::nextId() const {
    // Next ID = max existing numeric suffix + 1 (e.g. STU-013).
    int maxNum = 0;
    for (const Student &s : m_students) {
        bool ok = false;
        const int num = s.id.section('-', 1).toInt(&ok);
        if (ok)
            maxNum = qMax(maxNum, num);
    }
    return QString("STU-%1").arg(maxNum + 1, 3, 10, QChar('0'));
}

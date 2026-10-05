#pragma once

#include <QAbstractTableModel>

#include "Student.h"

// Read-only table model exposing the mock student list.
// Columns: ID | Full Name | Age | Gender
class StudentModel : public QAbstractTableModel {
    Q_OBJECT
public:
    enum Column { ColId = 0, ColName, ColAge, ColGender, ColCount };

    explicit StudentModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // CRUD operations (in-memory; proxy views update automatically).
    void addStudent(const Student &student);
    void updateStudent(int row, const Student &student);
    void removeStudent(int row);
    Student studentAt(int row) const;
    QString nextId() const;

private:
    QList<Student> m_students;
};

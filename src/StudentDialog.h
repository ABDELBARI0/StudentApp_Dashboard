#pragma once

#include <QDialog>

#include "Student.h"

class QComboBox;
class QLabel;
class QLineEdit;
class QSpinBox;

// Modal form used for both adding a new student and editing an existing one.
// For new students the ID is pre-generated and read-only.
class StudentDialog : public QDialog {
    Q_OBJECT
public:
    // Pass an existing student to edit, or a student with a fresh ID to add.
    explicit StudentDialog(const Student &student, bool isNew,
                           QWidget *parent = nullptr);

    Student student() const;

private slots:
    void onSave();

private:
    Student m_original;
    bool m_isNew = true;
    QLineEdit *m_idField = nullptr;
    QLineEdit *m_nameField = nullptr;
    QSpinBox *m_ageField = nullptr;
    QComboBox *m_genderField = nullptr;
    QLabel *m_errorLabel = nullptr;
};

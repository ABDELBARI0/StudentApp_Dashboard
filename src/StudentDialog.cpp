#include "StudentDialog.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

StudentDialog::StudentDialog(const Student &student, bool isNew,
                             QWidget *parent)
    : QDialog(parent), m_original(student), m_isNew(isNew) {
    setObjectName("StudentDialog");
    setWindowTitle(isNew ? "Add Student" : "Edit Student");
    setModal(true);
    setFixedWidth(400);

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 22, 24, 22);
    root->setSpacing(12);

    auto *title = new QLabel(isNew ? "Add Student" : "Edit Student", this);
    title->setObjectName("CardTitle");
    auto *subtitle = new QLabel(isNew ? "Enter the new student's information"
                                      : "Update the student's information",
                                this);
    subtitle->setObjectName("CardSubtitle");
    root->addWidget(title);
    root->addWidget(subtitle);

    auto *form = new QFormLayout();
    form->setSpacing(10);

    m_idField = new QLineEdit(student.id, this);
    m_idField->setObjectName("FormField");
    m_idField->setReadOnly(true);
    form->addRow("Student ID", m_idField);

    m_nameField = new QLineEdit(student.fullName, this);
    m_nameField->setObjectName("FormField");
    m_nameField->setPlaceholderText("e.g. John Smith");
    m_nameField->setMaxLength(60);
    form->addRow("Full Name *", m_nameField);

    m_ageField = new QSpinBox(this);
    m_ageField->setObjectName("FormField");
    m_ageField->setRange(15, 100);
    m_ageField->setValue(student.age > 0 ? student.age : 20);
    form->addRow("Age", m_ageField);

    m_genderField = new QComboBox(this);
    m_genderField->setObjectName("GenderCombo");
    m_genderField->addItems({"Male", "Female"});
    m_genderField->setCurrentIndex(
        student.gender == Student::Gender::Female ? 1 : 0);
    m_genderField->setCursor(Qt::PointingHandCursor);
    form->addRow("Gender", m_genderField);
    root->addLayout(form);

    m_errorLabel = new QLabel(this);
    m_errorLabel->setObjectName("ErrorLabel");
    m_errorLabel->hide();
    root->addWidget(m_errorLabel);

    auto *buttons = new QDialogButtonBox(this);
    auto *saveButton = buttons->addButton(
        isNew ? "Add Student" : "Save Changes", QDialogButtonBox::AcceptRole);
    saveButton->setObjectName("PrimaryButton");
    saveButton->setCursor(Qt::PointingHandCursor);
    auto *cancelButton = buttons->addButton(QDialogButtonBox::Cancel);
    cancelButton->setObjectName("ToolButton");
    cancelButton->setCursor(Qt::PointingHandCursor);
    connect(buttons, &QDialogButtonBox::accepted, this, &StudentDialog::onSave);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    root->addWidget(buttons);

    m_nameField->setFocus();
}

Student StudentDialog::student() const {
    return Student{
        m_idField->text(),
        m_nameField->text().trimmed(),
        m_ageField->value(),
        m_genderField->currentIndex() == 1 ? Student::Gender::Female
                                           : Student::Gender::Male,
    };
}

void StudentDialog::onSave() {
    if (m_nameField->text().trimmed().isEmpty()) {
        m_errorLabel->setText("Full name is required.");
        m_errorLabel->show();
        m_nameField->setFocus();
        return;
    }
    accept();
}

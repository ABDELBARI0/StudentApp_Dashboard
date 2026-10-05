#pragma once

#include <QList>
#include <QString>

// In-memory student record (UI mock data only — no persistence).
struct Student {
    enum class Gender { Male, Female };

    QString id;
    QString fullName;
    int age = 0;
    Gender gender = Gender::Male;

    QString genderText() const {
        return gender == Gender::Male ? "Male" : "Female";
    }

    static QList<Student> mockStudents() {
        return {
            {"STU-001", "John Smith", 20, Gender::Male},
            {"STU-002", "Sarah Wilson", 19, Gender::Female},
            {"STU-003", "Michael Brown", 21, Gender::Male},
            {"STU-004", "Emma Davis", 20, Gender::Female},
            {"STU-005", "James Miller", 22, Gender::Male},
            {"STU-006", "Olivia Taylor", 18, Gender::Female},
            {"STU-007", "Daniel Anderson", 21, Gender::Male},
            {"STU-008", "Sophia Thomas", 19, Gender::Female},
            {"STU-009", "David Jackson", 23, Gender::Male},
            {"STU-010", "Ava White", 20, Gender::Female},
            {"STU-011", "Chris Harris", 22, Gender::Male},
            {"STU-012", "Mia Martin", 18, Gender::Female},
        };
    }
};

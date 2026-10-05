#pragma once

#include <QWidget>

class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTableView;
class StudentModel;
class StudentProxy;

// Root window: toolbar (add + search + gender filter) above a table card
// with per-selection edit/delete actions.
class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onSearchChanged(const QString &text);
    void onGenderChanged(int index);
    void onAdd();
    void onEdit();
    void onDelete();
    void updateFooter();

private:
    int selectedSourceRow() const;
    void updateActionStates();
    void loadStylesheet();

    StudentModel *m_model = nullptr;
    StudentProxy *m_proxy = nullptr;
    QTableView *m_table = nullptr;
    QLineEdit *m_searchBox = nullptr;
    QComboBox *m_genderCombo = nullptr;
    QLabel *m_countLabel = nullptr;
    QLabel *m_emptyLabel = nullptr;
    QPushButton *m_editButton = nullptr;
    QPushButton *m_deleteButton = nullptr;
};

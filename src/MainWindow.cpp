#include "MainWindow.h"

#include <QComboBox>
#include <QFile>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QShortcut>
#include <QTableView>
#include <QVBoxLayout>

#include "GenderBadgeDelegate.h"
#include "StudentDialog.h"
#include "StudentModel.h"
#include "StudentProxy.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setObjectName("AppRoot");
    setWindowTitle("Student Records");
    resize(1100, 700);
    setMinimumSize(760, 520);
    loadStylesheet();

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // Toolbar: title on the left, search + gender filter on the right.
    auto *toolbar = new QWidget(this);
    toolbar->setObjectName("Toolbar");
    toolbar->setFixedHeight(76);
    auto *barLayout = new QHBoxLayout(toolbar);
    barLayout->setContentsMargins(28, 12, 28, 12);
    barLayout->setSpacing(12);

    auto *titleBox = new QVBoxLayout();
    titleBox->setSpacing(2);
    auto *title = new QLabel("Students", toolbar);
    title->setObjectName("PageTitle");
    auto *subtitle = new QLabel("Manage student information records", toolbar);
    subtitle->setObjectName("PageSubtitle");
    titleBox->addWidget(title);
    titleBox->addWidget(subtitle);
    barLayout->addLayout(titleBox);
    barLayout->addStretch();

    auto *addButton = new QPushButton("+  Add Student", toolbar);
    addButton->setObjectName("PrimaryButton");
    addButton->setCursor(Qt::PointingHandCursor);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAdd);
    barLayout->addWidget(addButton);

    m_searchBox = new QLineEdit(toolbar);
    m_searchBox->setObjectName("SearchBox");
    m_searchBox->setPlaceholderText("Search by ID or name...");
    m_searchBox->setClearButtonEnabled(true);
    connect(m_searchBox, &QLineEdit::textChanged,
            this, &MainWindow::onSearchChanged);
    barLayout->addWidget(m_searchBox);

    m_genderCombo = new QComboBox(toolbar);
    m_genderCombo->setObjectName("GenderCombo");
    m_genderCombo->addItems({"All Genders", "Male", "Female"});
    m_genderCombo->setCursor(Qt::PointingHandCursor);
    connect(m_genderCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onGenderChanged);
    barLayout->addWidget(m_genderCombo);
    root->addWidget(toolbar);

    // Content area with the table card.
    auto *content = new QWidget(this);
    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(28, 24, 28, 28);
    root->addWidget(content, 1);

    auto *card = new QFrame(content);
    card->setObjectName("Card");
    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(22, 20, 22, 20);
    cardLayout->setSpacing(12);

    auto *cardHeader = new QHBoxLayout();
    cardHeader->setSpacing(8);
    auto *cardTitleBox = new QVBoxLayout();
    cardTitleBox->setSpacing(3);
    auto *cardTitle = new QLabel("All Students", card);
    cardTitle->setObjectName("CardTitle");
    auto *cardSubtitle = new QLabel("Click a column header to sort", card);
    cardSubtitle->setObjectName("CardSubtitle");
    cardTitleBox->addWidget(cardTitle);
    cardTitleBox->addWidget(cardSubtitle);
    cardHeader->addLayout(cardTitleBox);
    cardHeader->addStretch();
    m_countLabel = new QLabel(card);
    m_countLabel->setObjectName("RecordCount");
    cardHeader->addWidget(m_countLabel);

    m_editButton = new QPushButton("Edit", card);
    m_editButton->setObjectName("ToolButton");
    m_editButton->setCursor(Qt::PointingHandCursor);
    m_editButton->setEnabled(false);
    connect(m_editButton, &QPushButton::clicked, this, &MainWindow::onEdit);
    cardHeader->addWidget(m_editButton);

    m_deleteButton = new QPushButton("Delete", card);
    m_deleteButton->setObjectName("DangerButton");
    m_deleteButton->setCursor(Qt::PointingHandCursor);
    m_deleteButton->setEnabled(false);
    connect(m_deleteButton, &QPushButton::clicked, this, &MainWindow::onDelete);
    cardHeader->addWidget(m_deleteButton);
    cardLayout->addLayout(cardHeader);

    m_model = new StudentModel(this);
    m_proxy = new StudentProxy(this);
    m_proxy->setSourceModel(m_model);

    m_table = new QTableView(card);
    m_table->setObjectName("StudentTable");
    m_table->setModel(m_proxy);
    m_table->setSortingEnabled(true);
    m_table->sortByColumn(StudentModel::ColId, Qt::AscendingOrder);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->verticalHeader()->setDefaultSectionSize(44);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->horizontalHeader()->setHighlightSections(false);
    m_table->setItemDelegateForColumn(StudentModel::ColGender,
                                      new GenderBadgeDelegate(m_table));
    cardLayout->addWidget(m_table, 1);

    // Selection-driven actions: double-click edits, Delete key deletes.
    connect(m_table->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, [this]() { updateActionStates(); });
    connect(m_table, &QTableView::doubleClicked, this, &MainWindow::onEdit);
    auto *deleteShortcut = new QShortcut(QKeySequence::Delete, m_table);
    deleteShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(deleteShortcut, &QShortcut::activated, this, &MainWindow::onDelete);

    m_emptyLabel = new QLabel("No students match your search.", card);
    m_emptyLabel->setObjectName("EmptyState");
    m_emptyLabel->setAlignment(Qt::AlignCenter);
    m_emptyLabel->hide();
    cardLayout->addWidget(m_emptyLabel);

    contentLayout->addWidget(card, 1);

    connect(m_proxy, &QAbstractItemModel::rowsInserted,
            this, &MainWindow::updateFooter);
    connect(m_proxy, &QAbstractItemModel::rowsRemoved,
            this, &MainWindow::updateFooter);
    connect(m_proxy, &QAbstractItemModel::modelReset,
            this, &MainWindow::updateFooter);
    connect(m_proxy, &QAbstractItemModel::layoutChanged,
            this, &MainWindow::updateFooter);
    updateFooter();
}

void MainWindow::onSearchChanged(const QString &text) {
    m_proxy->setSearchText(text.trimmed());
    updateFooter();
}

void MainWindow::onGenderChanged(int index) {
    m_proxy->setGenderFilter(static_cast<StudentProxy::GenderFilter>(index));
    updateFooter();
}

void MainWindow::onAdd() {
    Student fresh;
    fresh.id = m_model->nextId();
    fresh.age = 20;
    StudentDialog dialog(fresh, true, this);
    if (dialog.exec() == QDialog::Accepted) {
        m_model->addStudent(dialog.student());
        updateFooter();
    }
}

void MainWindow::onEdit() {
    const int row = selectedSourceRow();
    if (row < 0)
        return;
    StudentDialog dialog(m_model->studentAt(row), false, this);
    if (dialog.exec() == QDialog::Accepted) {
        m_model->updateStudent(row, dialog.student());
        updateFooter();
    }
}

void MainWindow::onDelete() {
    const int row = selectedSourceRow();
    if (row < 0)
        return;
    const Student s = m_model->studentAt(row);
    const auto answer = QMessageBox::question(
        this, "Delete Student",
        QString("Delete %1 (%2)?").arg(s.fullName, s.id),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (answer == QMessageBox::Yes) {
        m_model->removeStudent(row);
        updateFooter();
    }
}

int MainWindow::selectedSourceRow() const {
    const QModelIndexList selected =
        m_table->selectionModel()->selectedRows();
    if (selected.isEmpty())
        return -1;
    return m_proxy->mapToSource(selected.first()).row();
}

void MainWindow::updateActionStates() {
    const bool hasSelection = selectedSourceRow() >= 0;
    m_editButton->setEnabled(hasSelection);
    m_deleteButton->setEnabled(hasSelection);
}

void MainWindow::updateFooter() {
    const int shown = m_proxy->rowCount();
    const int total = m_model->rowCount();
    m_countLabel->setText(
        shown == total ? QString("%1 records").arg(total)
                       : QString("%1 of %2 records").arg(shown).arg(total));
    m_emptyLabel->setVisible(shown == 0);
    updateActionStates();
}

void MainWindow::loadStylesheet() {
    QFile file(":/styles/students.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        setStyleSheet(QString::fromUtf8(file.readAll()));
}

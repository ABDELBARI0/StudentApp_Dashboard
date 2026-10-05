#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("Student Records");
    QApplication::setOrganizationName("StudentApp");

    MainWindow window;
    window.show();
    return app.exec();
}

# Student Records (Qt 6, UI Only)

Desktop UI for viewing student information records, built with C++17,
Qt 6 Widgets, CMake, and centralized QSS styling.

UI only — all records are hardcoded mock data held in memory; there is no
database, file storage, API, or backend. Records reset when the app restarts.

## Requirements

- Qt 6 (Widgets)
- CMake 3.19+
- C++17 compiler (MSVC, MinGW, GCC, or Clang)

Windows (MSYS2 UCRT64) example:

```bash
pacman -S mingw-w64-ucrt-x86_64-qt6-base \
          mingw-w64-ucrt-x86_64-qt6-tools \
          mingw-w64-ucrt-x86_64-cmake \
          mingw-w64-ucrt-x86_64-ninja
```

## Build & Run

```bash
cmake -S StudentApp -B StudentApp/build -G Ninja \
  -DCMAKE_PREFIX_PATH=C:/msys64/ucrt64
cmake --build StudentApp/build
./StudentApp/build/StudentApp.exe
```

(Adjust `CMAKE_PREFIX_PATH` to your Qt 6 install location. When launching
from a terminal, make sure the Qt `bin` directory is on `PATH`.)

## Project Structure

```text
StudentApp/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── Student.h                # record struct + mock data
│   ├── StudentModel.h / .cpp    # QAbstractTableModel (ID, name, age, gender)
│   ├── StudentProxy.h / .cpp    # search + gender filtering, sorting support
│   ├── GenderBadgeDelegate.h    # pill badge painter for the Gender column
│   │   └── .cpp
│   ├── StudentDialog.h / .cpp    # modal add/edit form with validation
│   ├── MainWindow.h / .cpp      # toolbar (add/search/filter) + table card
├── resources/
│   ├── resources.qrc
│   └── styles/students.qss      # centralized dark theme
└── README.md
```

## Features

- Add students via a validated dialog (ID auto-generated, e.g. STU-013)
- Edit the selected student (Edit button or double-click a row)
- Delete the selected student (Delete button or Delete key, with confirmation)
- Sortable student table (click any column header)
- Live search across student ID and full name
- Gender filter (All / Male / Female) with record counter
- Empty-state message when no records match

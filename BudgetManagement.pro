QT += core gui
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    budget.cpp \
    category.cpp \
    databaseManager.cpp \
    expense.cpp \
    income.cpp \
    item.cpp \
    main.cpp \
    mainwindow.cpp \
    saving.cpp

HEADERS += \
    budget.h \
    category.h \
    databaseManager.h \
    expense.h \
    income.h \
    item.h \
    mainwindow.h \
    saving.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



# win32: LIBS += -L$$PWD/sqlite3/ -lsqlite3

# INCLUDEPATH += $$PWD/sqlite3
# DEPENDPATH += $$PWD/sqlite3

RESOURCES += \
    resource.qrc

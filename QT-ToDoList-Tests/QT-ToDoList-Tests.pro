QT += testlib
QT -= gui
QT += widgets
QT += core gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
            ../QT-ToDoList-App/createdbfile.cpp \
            ../QT-ToDoList-App/iomanager.cpp \
            ../QT-ToDoList-App/mainwindow.cpp \
            ../QT-ToDoList-App/taskmanager.cpp \
            ../QT-ToDoList-App/listadder.cpp \
            ../QT-ToDoList-App/listdeleter.cpp \
            ../QT-ToDoList-App/task.cpp \
            ../QT-ToDoList-App/tasklist.cpp \
            iomanagertest.cpp \
            main.cpp \
            mainwindowtest.cpp \
            taskmanagertest.cpp

HEADERS += \
    ../QT-ToDoList-App/createdbfile.h \
    ../QT-ToDoList-App/iomanager.h \
    ../QT-ToDoList-App/mainwindow.h \
    ../QT-ToDoList-App/taskmanager.h \
    ../QT-ToDoList-App/listadder.h \
    ../QT-ToDoList-App/listdeleter.h \
    ../QT-ToDoList-App/task.h \
    ../QT-ToDoList-App/tasklist.h \
    iomanagertest.h \
    mainwindowtest.h \
    taskmanagertest.h

FORMS += \
    ../QT-ToDoList-App/createdbfile.ui \
    ../QT-ToDoList-App/mainwindow.ui \
    ../QT-ToDoList-App/listadder.ui \
    ../QT-ToDoList-App/listdeleter.ui \
    ../QT-ToDoList-App/taskmanager.ui

INCLUDEPATH += $$PWD/../QT-ToDoList-App
DEPENDPATH += $$PWD/../QT-ToDoList-App

copydata.commands = $(COPY_DIR) $$PWD/RegularDbTest.cfg $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

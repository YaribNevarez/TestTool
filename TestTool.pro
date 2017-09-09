#-------------------------------------------------
#
# Project created by QtCreator 2017-09-05T22:50:52
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestTool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    deviceform.cpp

HEADERS  += mainwindow.h \
    deviceform.h

FORMS    += mainwindow.ui \
    deviceform.ui

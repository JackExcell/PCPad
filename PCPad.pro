#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T00:41:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PCPad
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    controller.cpp

HEADERS  += mainwindow.h \
    controller.h

FORMS    += mainwindow.ui

win32: LIBS += -lXInput

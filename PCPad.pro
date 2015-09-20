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
    xinputstateworker.cpp

HEADERS  += mainwindow.h \
    xinputstateworker.h

FORMS    += mainwindow.ui

win32: LIBS += -lXInput

RESOURCES += \
    pcpadresources.qrc

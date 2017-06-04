#-------------------------------------------------
#
# Project created by QtCreator 2015-08-31T09:45:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QWinTaskbarButton_progress
TEMPLATE = app

win32: {
    QT += winextras
    DEFINES += USE_WIN_EXTRAS
}

CONFIG(release, debug|release): {
    DEFINES += QT_NO_DEBUG_OUTPUT
    DEFINES += MYPREFIX_RELEASE
} else {
    DEFINES += MYPREFIX_DEBUG
}

DEFINES += QT_USE_QSTRINGBUILDER
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExampleProject
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_USE_QSTRINGBUILDER
CONFIG += c++11

CONFIG(release, debug|release): {
    DEFINES += QT_NO_DEBUG
    DEFINES += QT_NO_DEBUG_OUTPUT
    DEFINES += MYPREFIX_RELEASE
} else {
    DEFINES += MYPREFIX_DEBUG
}

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        src/main.cpp \
        src/gui/mainwindow.cpp \


HEADERS += \
        src/gui/mainwindow.h \


FORMS += \
        src/gui/mainwindow.ui

TRANSLATIONS += translations/ExampleProject_ru.ts

VERSION = 0.0.0.1
#QMAKE_TARGET_COMPANY = company
QMAKE_TARGET_PRODUCT = ExampleProject
QMAKE_TARGET_DESCRIPTION = "ExampleProject app."
QMAKE_TARGET_COPYRIGHT = (c) Yuriy Astrov
# RC_ICONS = myappico.ico
# RC_FILE = myapp.rc
GITHUB_URL = "https://github.com/yastrov/ExampleProject"

DEFINES += \
APP_VERSION=\"\\\"$$VERSION\\\"\" \
APP_URL=\"\\\"$$GITHUB_URL\\\"\" \

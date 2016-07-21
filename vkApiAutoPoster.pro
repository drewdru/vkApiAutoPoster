#-------------------------------------------------
#
# Project created by QtCreator 2015-07-04T16:39:21
#
#-------------------------------------------------

QT += core gui
QT += webkitwidgets
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fantasyinpictures
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    webpageparser.cpp \
    Settings.cpp \
    mainWdb.cpp \
    viewCurrentPost.cpp \
    ViewWebPage.cpp

HEADERS  += mainwindow.h \
    webpageparser.h \
    Settings.h \
    viewCurrentPost.h \
    ViewWebPage.h

FORMS    += mainwindow.ui \
    Settings.ui \
    viewCurrentPost.ui \
    ViewWebPage.ui

RESOURCES += \
    img.qrc

#-------------------------------------------------
#
# Project created by QtCreator 2015-03-30T14:13:53
#
#-------------------------------------------------

QT       += core gui axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qcg
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    initesri.cpp \
    esrigeodatabase.cpp

HEADERS  += mainwindow.h \
    InitESRI.h \
    qgeodatabase.h \
    qcommon.h \
    esrigeodatabase.h

FORMS    += mainwindow.ui


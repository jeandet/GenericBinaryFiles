#-------------------------------------------------
#
# Project created by QtCreator 2016-09-05T22:43:01
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_testfragmentstest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
INCLUDEPATH += ../../src

LIBS += -L../../src/bin -lgenericBinaryFiles

SOURCES += tst_testfragmentstest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

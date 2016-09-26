#-------------------------------------------------
#
# Project created by QtCreator 2016-09-05T17:11:26
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_testatest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../src

LIBS += -L../../src/bin -lgenericBinaryFiles

SOURCES += tst_testatest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

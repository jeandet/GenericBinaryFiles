#-------------------------------------------------
#
# Project created by QtCreator 2016-09-06T23:35:17
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_testsrectest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
INCLUDEPATH += ../../src

LIBS += -L../../src/bin -lgenericBinaryFiles

OTHER_FILES += sample.srec WrongCRC.srec WrongRecordType.srec

SOURCES += tst_testsrectest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

QT       += core

TARGET = genericBinaryFiles
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++14

MOC_DIR      = moc
RCC_DIR      = resources
OBJECTS_DIR  = obj
DESTDIR      = bin

#INCLUDEPATH += 

SOURCES += \ 
    binaryfile.cpp \
    binaryfileio.cpp \
    srec/srecfileio.cpp


HEADERS  += \ 
    binaryfile.h \
    binaryfileio.h \
    srec/srecfileio.h \
    segment.h \
    private/binaryfiledataprivate.h

OTHER_FILES += 

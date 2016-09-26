#ifndef BINARYFILEIO_H
#define BINARYFILEIO_H
#include <QString>
#include <QVector>
#include "segment.h"
class BinaryFileDataPrivate;


class BinaryFileIO
{
public:
    BinaryFileIO(){}
    virtual ~BinaryFileIO();
    virtual BinaryFileDataPrivate read(const QString& fileName)=0;

    virtual bool write(const QString& fileName, const BinaryFileDataPrivate& data)=0;
};


typedef BinaryFileIO* (*BinaryFileIO_ctor)();


#endif // BINARYFILEIO_H


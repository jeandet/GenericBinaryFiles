#ifndef BINARYFILE_H
#define BINARYFILE_H
#include <QString>
#include <QVector>
#include <QScopedPointer>
#include <stdexcept>
#include <iostream>
#include "segment.h"
class BinaryFileDataPrivate;

class BinaryFile
{
public:
    BinaryFile();
    bool load(const QString& fileName, const QString& type="srec");
    bool save(const QString& fileName, const QString& type="srec");
    const QVector<Segment>& segments();

private:
    QScopedPointer<BinaryFileDataPrivate> p_data;
};

#endif // BINARYFILE_H

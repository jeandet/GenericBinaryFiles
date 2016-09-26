#include "binaryfile.h"
#include <QHash>
#include <QFile>
#include "binaryfileio.h"
#include <private/binaryfiledataprivate.h>

extern BinaryFileIO* SrecFileIO_ctor();

const QHash<QString,BinaryFileIO_ctor> p_formats_ctors_LUT={
    {"srec",SrecFileIO_ctor}
};



BinaryFile::BinaryFile()
    :p_data(new BinaryFileDataPrivate())
{

}

/**
 * @brief BinaryFile::open Open given file with matching decoder.
 * @param fileName Path to an exisitng file.
 * @param type File type, might be "srec","bin","elf" or "ihex"
 * @return True if successfully opened and loaded
 */
bool BinaryFile::load(const QString &fileName, const QString& type)
{
    if(Q_UNLIKELY(!QFile::exists(fileName)))
        return false;
    if(p_formats_ctors_LUT.contains(type))
    {
        QScopedPointer<BinaryFileIO> file(p_formats_ctors_LUT[type]());
        *this->p_data.data() =  file.data()->read(fileName);
    }
    return false;
}

bool BinaryFile::save(const QString &fileName, const QString &type)
{
    if(p_formats_ctors_LUT.contains(type))
    {
        QScopedPointer<BinaryFileIO> file(p_formats_ctors_LUT[type]());
    }
    return false;
}

const QVector<Segment> &BinaryFile::segments()
{
    return this->p_data.data()->segments();
}


BinaryFileDataPrivate::BinaryFileDataPrivate()
{

}

BinaryFileDataPrivate::~BinaryFileDataPrivate()
{

}

void BinaryFileDataPrivate::addSegment(const Segment &segment)
{
    this->p_segments.append(segment);
}

void BinaryFileDataPrivate::addSegment(Segment &&segment)
{
    this->p_segments.append(std::move(segment));
}

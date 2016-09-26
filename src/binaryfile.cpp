/*------------------------------------------------------------------------------
--  This file is a part of the GenericBinaryFiles library
--  Copyright (C) 2016, Plasma Physics Laboratory - CNRS
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this program; if not, write to the Free Software
--  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
-------------------------------------------------------------------------------*/
/*--                  Author : Alexis Jeandet
--                     Mail : alexis.jeandet@member.fsf.org
----------------------------------------------------------------------------*/
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

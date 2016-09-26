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


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

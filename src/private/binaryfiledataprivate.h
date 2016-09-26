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

#ifndef BINARYFILEDATAPRIVATE_H
#define BINARYFILEDATAPRIVATE_H
#include <segment.h>
#include <QVector>

class BinaryFileDataPrivate
{
public:
    BinaryFileDataPrivate();
    ~BinaryFileDataPrivate();
    void addSegment(const Segment& segment);
    void addSegment(Segment&& segment);
    const QVector<Segment>& segments(){return  this->p_segments;}
private:
    QVector<Segment> p_segments;
};

#endif // BINARYFILEDATAPRIVATE_H

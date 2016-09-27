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
#ifndef SRECFILEIO_H
#define SRECFILEIO_H
#include <binaryfileio.h>
#include <QIODevice>

class SrecFileIO : public BinaryFileIO
{
private:
    QString p_currentFileName;
    int p_currentLine;

public:
    SrecFileIO();
    virtual ~SrecFileIO(){}
    virtual BinaryFileDataPrivate read(const QString& fileName) final;
    virtual bool write(const QString& fileName, const BinaryFileDataPrivate& data) final;

private:
    Segment parseLine(const QByteArray& line);
    std::vector<char> p_loadLine(const QByteArray& line, int size, int dataOffset);
    bool p_writeRecord(QIODevice& device,int recordType,int address,const QByteArray& data);
    inline QByteArray p_make_record(int type,int address,const QByteArray& data);
    int p_CRC(const QByteArray &line, int size);
};

BinaryFileIO* SrecFileIO_ctor(){return static_cast<BinaryFileIO*>(new SrecFileIO());}

#endif // SRECFILEIO_H

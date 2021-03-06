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
#include "srecfileio.h"
#include <QFile>
#include "private/binaryfiledataprivate.h"

//Simple and fast ascii to int lookup table
// hextable[ascii char] returns its integer representation
static const int hextable[] = {-1,-1,
  -1,  -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1, -1,
   10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
   10, 11,12,13,14,15
};

SrecFileIO::SrecFileIO()
{

}

static inline int p_addressSize(int recordType)
{
    int size=0;
    switch (recordType)
    {
    case 0: case 1: case 5: case 9:   //16 bits address
        size=2;
        break;
    case 3: case 7:   //32 bits address
        size=4;
        break;
    default: //24 bits address
        size=3;
        break;
    }
    return  size;
}


inline QByteArray SrecFileIO::p_make_record(int type,int address,const QByteArray& data)
{
    int addressSize=p_addressSize(type);
    QByteArray record(2+255,'\0');
    record[0]='S';
    record[1]=static_cast<char>(0x30&type);
    for(int i=0;i<addressSize;i++)
    {
        record[2+i]=static_cast<char>(address>>((addressSize-i-1)*8));
    }
    record.replace(2+addressSize,data.size(),data);
    record[2+255]=static_cast<char>(p_CRC(record,255));
    return  record;
}

static inline int hexCharToInt(char MSB,char LSB)
{
    return  (hextable[static_cast<int>(MSB)]<<4 )+ hextable[static_cast<int>(LSB)];
}

inline int hexCharToAddress(const QByteArray& data,int offset, int addressBits=16)
{
    int address=0;
    while(addressBits>0)
    {
        addressBits-=8;
        address<<=8;
        address += hexCharToInt(data[offset],data[offset+1]);
        offset+=2;
    }
    return address;
}

BinaryFileDataPrivate SrecFileIO::read(const QString &fileName)
{
    BinaryFileDataPrivate data;
    QFile srecFile(fileName);
    if(srecFile.open(QIODevice::ReadOnly))
    {
        this->p_currentFileName=fileName;
        this->p_currentLine=0;
        while(!srecFile.atEnd())
        {
            QByteArray line=srecFile.readLine();
            data.addSegment(parseLine(line));
            this->p_currentLine+=1;
        }
    }
    return  data;
}


// TODO Test ME!
bool SrecFileIO::write(const QString &fileName, const BinaryFileDataPrivate &data)
{
    QFile outfile(fileName);
    if(outfile.open(QIODevice::WriteOnly))
    {
        p_writeRecord(outfile,0,0,fileName.toLatin1());
        for(const auto& segment : data.segments())
        {

            p_writeRecord(outfile,3,static_cast<int>(segment.address()),segment.data());
        }
    }
    return true;
}

Segment SrecFileIO::parseLine(const QByteArray &line)
{
    if(line.length()>=12 && line[0]=='S')
    {
        int address=0;
        int recordType = line[1]&0x0F;
        int dataOffset=0;
        int byteCount=hexCharToInt(line[2],line[3]);
        if(recordType >9)
        {
            QString msg=
                    QString("Record type out of range(0-9) at line %1 got value %2").arg(this->p_currentLine,recordType);
            throw std::runtime_error(msg.toStdString());
        }
        int CRC=hexCharToInt(line[(byteCount*2)+2],line[(byteCount*2)+3]);
        int computedCRC=p_CRC(line,byteCount*2);
        if(Q_UNLIKELY(CRC!=computedCRC))
        {
            QString msg=
                    QString("Wrong CRC at line %1 got value %2 expected %3").arg(this->p_currentLine,computedCRC,CRC);
            throw std::runtime_error(msg.toStdString());
        }
        int addressSize=p_addressSize(recordType);
        address=hexCharToAddress(line,4,addressSize*8);
        dataOffset=4+(addressSize*2);
        std::vector<char> data = p_loadLine(line,(byteCount*2)-(dataOffset-2),dataOffset);
        return  Segment(std::move(data),static_cast<quint64>(address),"");
    }
    else
    {
        throw std::runtime_error("Bad Srec File");
    }
}

std::vector<char> SrecFileIO::p_loadLine(const QByteArray &line, int size, int dataOffset)
{
    std::vector<char> data(static_cast<size_t>(size/2));
    for(int  i=0;i<size;i+=2)
        data[static_cast<size_t>(i/2)]=static_cast<char>(hexCharToInt(line[i+dataOffset],line[i+dataOffset+1]));
    return  data;
}


// TODO test ME!
bool SrecFileIO::p_writeRecord(QIODevice &device, int recordType, int address, const QByteArray &data)
{
    //Record size is 8 bit = address(2|3|4 bytes) + data + CRC(1 byte)
    int addressSize=p_addressSize(recordType);
    int chunkSize=255-addressSize-1;
    int fullChunksCount=data.length()/(chunkSize);
    int lastChunkSize=data.length()%(chunkSize);
    for(int i=0;i<fullChunksCount;i++)
    {
        device.write(p_make_record(recordType,address+(i*chunkSize),data.mid(i*chunkSize,chunkSize)));
    }
    if(lastChunkSize)
    {
        device.write(p_make_record(recordType,address+(fullChunksCount*chunkSize),data.mid(fullChunksCount*chunkSize,lastChunkSize)));
    }
    return true;
}

int SrecFileIO::p_CRC(const QByteArray &line, int size)
{
    int CRC=0;
    for(int i=2;i<size+2;i+=2)
        CRC+=hexCharToInt(line[i],line[i+1]);
    CRC=0xFF&~CRC;
    return CRC;
}

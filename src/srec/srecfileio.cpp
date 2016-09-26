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

static inline int hexCharToInt(char MSB,char LSB)
{
    return  (hextable[(int)MSB]<<4 )+ hextable[(int)LSB];
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

bool SrecFileIO::write(const QString &fileName, const BinaryFileDataPrivate &data)
{

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
        return  Segment(std::move(data),address,"");
    }
    else
    {
        throw std::runtime_error("Bad Srec File");
    }
}

std::vector<char> SrecFileIO::p_loadLine(const QByteArray &line, int size, int dataOffset)
{
    std::vector<char> data(static_cast<size_t>(size/2));
    for(int i=0;i<size;i+=2)
        data[i/2]=static_cast<char>(hexCharToInt(line[i+dataOffset],line[i+dataOffset+1]));
    return  data;
}

bool SrecFileIO::p_writeHeader(QIODevice &device, const QString &header)
{
    return p_writeRecord(device,0,0,header.toLatin1());
}


// TODO complete ME!
bool SrecFileIO::p_writeRecord(QIODevice &device, int recordType, int address, const QByteArray &data)
{
    int addressSize=p_addressSize(recordType);
    QByteArray addressBA(addressSize,'0');
    for(int i;i<addressSize;i++)
    {
        addressBA[i]=static_cast<char>(address>>((addressSize-i-1)*8));
    }
    //Record size is 8 bit = address(2|3|4 bytes) + data + CRC(1 byte)
    int chunkSize=255-addressSize-1;
    int fullChunksCount=data.length()/(chunkSize);
    int lastChunkSize=data.length()%(chunkSize);
    for(int i=0;i<fullChunksCount;i++)
    {
        QByteArray record(2+255,'\0');
        std::copy(data.constBegin()+(i*chunkSize),data.constBegin()+((i+1)*chunkSize),record.begin()+2+addressSize);
        record[0]='S';
        record[1]=static_cast<char>(0x30&recordType);

        record[2+255]=static_cast<char>(p_CRC(record,255));
        device.write(record);
    }
    if(lastChunkSize)
    {
        QByteArray record(2+lastChunkSize,'\0');
        //std::copy(data.constBegin()+((fullChunksCount-1)*chunkSize),data.constBegin()((fullChunksCount-1)*chunkSize)+lastChunkSize,record.begin()+2+addressSize);
        record[0]='S';
        record[1]=static_cast<char>(0x30&recordType);
        record[2+255]=static_cast<char>(p_CRC(record,lastChunkSize));
        device.write(record);
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

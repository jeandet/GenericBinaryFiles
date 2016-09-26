#ifndef SRECFILEIO_H
#define SRECFILEIO_H
#include <binaryfileio.h>
#include <QIODevice>

class SrecFileIO : public BinaryFileIO
{
public:
    SrecFileIO();
    virtual ~SrecFileIO(){}
    virtual BinaryFileDataPrivate read(const QString& fileName) final;
    virtual bool write(const QString& fileName, const BinaryFileDataPrivate& data) final;
private:
    Segment parseLine(const QByteArray& line);
    std::vector<char> p_loadLine(const QByteArray& line, int size, int dataOffset);
    bool p_writeHeader(QIODevice& device,const QString& header);
    bool p_writeRecord(QIODevice& device,int recordType,int address,const QByteArray& data);
    int p_CRC(const QByteArray &line, int size);
    QString p_currentFileName;
    int p_currentLine;
};

BinaryFileIO* SrecFileIO_ctor(){return static_cast<BinaryFileIO*>(new SrecFileIO());}

#endif // SRECFILEIO_H

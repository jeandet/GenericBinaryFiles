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

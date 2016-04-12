#ifndef DESCRIPTORFACTORY_H
#define DESCRIPTORFACTORY_H
#include <Descriptor.h>
#include <vector>
#include <math.h>
#include <QPainter>

class DescriptorFactory
{
private:
    MyQImage L;
    MyQImage Pfi;

public:
    DescriptorFactory(const MyQImage& image);
    Descriptor getDescrIntrPoint(const InterestingPoint point);
    Descriptor getNormalDescrIntrPoint(const InterestingPoint point);
};

#endif // DESCRIPTORFACTORY_H

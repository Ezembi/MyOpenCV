#ifndef DESCRIPTORFACTORY_H
#define DESCRIPTORFACTORY_H
#include <Descriptor.h>
#include <vector>
#include <math.h>
#include <QPainter>

class DescriptorFactory
{
private:
    std::vector<Descriptor> descriptors;
    MyQImage L;
    MyQImage Pfi;

public:
    DescriptorFactory(const MyQImage& image, int nIntrstingPoint);
    int getDescriptorSize() const;
    double getDescriptorsDistanse(const int dNumber, const Descriptor& descriptor) const;
    const Descriptor& getDescriptor(int i);

private:
    Descriptor getDescrIntrPoint(InterestingPoint point);
};

#endif // DESCRIPTORFACTORY_H

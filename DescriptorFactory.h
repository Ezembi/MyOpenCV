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
    int octave_;
    int level_;

public:
    DescriptorFactory(const MyQImage& image);
    DescriptorFactory(const MyQImage& image, int octave, int level);
    std::vector<Descriptor> getDescriptors(const std::vector<InterestingPoint> &_points);
    Descriptor getDescrIntrPoint(const InterestingPoint point);

private:
    //получим набор точек интереса со значениями поворота
    std::vector<InterestingPoint> getOrientation(const std::vector<InterestingPoint> &_points);
    std::pair<InterestingPoint, InterestingPoint> getOrientationIntrPoint(const InterestingPoint &_points);
};

#endif // DESCRIPTORFACTORY_H

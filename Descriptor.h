#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H
#include<vector>
#include<MyQImage.h>

class Descriptor
{
private:
    double gist_[nBin * nGistBin];       //гистограмма
    InterestingPoint point_;
public:
    Descriptor(InterestingPoint point);
    void setGistValue(int nGist, int bin, double value);
    void addGistValue(int nGist, int bin, double addValue);
    double getGistValue(int nGist, int bin) const;

    InterestingPoint getInterestingPoint() const;

    double getDistance(const Descriptor& descriptor) const;

    void normalize();
    double getVectorSize();

    void print();

};

#endif // DESCRIPTOR_H

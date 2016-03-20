#ifndef PYRAMID_H
#define PYRAMID_H
#include<PyramidOctave.h>

class Pyramid
{
public:
    std::vector<PyramidOctave> octaves;     //октавы
private:
    int k;                                  //2^(1/s)
    int sigma0;                             //нулевая сигма

public:
    Pyramid(MyQImage& resultImage, const double sigma0 , const int octave, const double numLevel);
    std::vector<PyramidOctave> getOctaves() const;
    void savePyramid(const QString path) const;
    void addOctrve(PyramidOctave& octave);
    int nOctave() const;
};

#endif // PYRAMID_H

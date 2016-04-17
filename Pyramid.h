#ifndef PYRAMID_H
#define PYRAMID_H
#include<PyramidOctave.h>

class Pyramid
{
private:
    std::vector<PyramidOctave> octaves;         //октавы
    double k;                                   //2^(1/s)
    double sigma0;                              //нулевая сигма

public:
    Pyramid(const MyQImage& resultImage, const double sigma0 , const int octave, const double numLevel);
    void savePyramid(const QString path) const;
    void addOctrve(PyramidOctave& octave);
    int nOctave() const;
    MyQImage getImage(int nOctav, int nLevel) const;
    std::vector<std::vector<Blob>> getBlobs();                                 //DoG для всей пирамиды и перезаписи пирамиды
};

#endif // PYRAMID_H

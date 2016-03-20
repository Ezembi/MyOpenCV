#ifndef PYRAMIDLEVEL_H
#define PYRAMIDLEVEL_H
#include <MyQImage.h>

class PyramidLevel
{
public:
    MyQImage image_;        //изображение
    double localSigma_;     //локальноая сигма для данного изображения
    double globalSigma_;    //глобальная сигма для данного уровня
    int n_;                 //номер уровня

public:
    PyramidLevel(const MyQImage& image, double localSigma, double globalSigma, int levelNumber);
    MyQImage getImage() const;
    double getLocalSigma() const;
    double getGlobalSigma() const;
    int getLevelNumber() const;
};

#endif // PYRAMIDLEVEL_H

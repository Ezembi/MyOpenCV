#ifndef PYRAMIDLEVEL_H
#define PYRAMIDLEVEL_H
#include <MyQImage.h>

class PyramidLevel
{
private:
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
    int getWidth() const;
    int getHeight() const;
    bool isExtremum(double value,
                    int x0,
                    int y0,
                    int x1,
                    int y1,
                    bool flag
                    );
    double getPixel(int x, int y);
    void Subtraction(const PyramidLevel& level);    //вычитание из данного уровня "level" и запись результата в данный уровень
};

#endif // PYRAMIDLEVEL_H
